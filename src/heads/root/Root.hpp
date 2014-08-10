#pragma once


#include <map>
#include <utility>

#include <rod/AsContextual.hpp>
#include <rod/AsSingleton.hpp>
#include <rod/Bind.hpp>
#include <rod/Contextual.hpp>
#include <rod/Dispatcher.hpp>
#include <rod/TypeList.hpp>

#include <QBuffer>
#include <QDataStream>
#include <QString>

#include <heads/Reducibles.hpp>
#include <heads/annotation/RootDescriptor.hpp>
#include <heads/common/QueryIdProvider.hpp>
#include <heads/common/RequestPool.hpp>
#include <heads/common/Socket.hpp>
#include <heads/root/ConnectionPreRegister.hpp>
#include <heads/root/ConnectionRegisterer.hpp>
#include <heads/root/HeadListener.hpp>
#include <heads/root/HeadListeners.hpp>
#include <heads/root/HeadReduceService.hpp>
#include <heads/root/Reducible.hpp>
#include <heads/root/Reductions.hpp>
#include <heads/root/RootServer.hpp>
#include <heads/root/annotation/ReducibleDescriptor.hpp>




namespace heads {
namespace root
{

	namespace rootDetail
	{
		template< typename Root, typename Listeners, typename ReduceService >
		class HeadConnectionRegisterer:
			public ConnectionRegisterer
		{
		private:
			Root*			root;
			QApplication&	app;
			Listeners&		listeners;
			ReduceService&	reduceService;


		public:
			HeadConnectionRegisterer( Root* root, QApplication& app, Listeners& listeners, ReduceService& reduceService ):
			  root( root ),
			  app( app ),
			  listeners( listeners ),
			  reduceService( reduceService )
			{}

			virtual
			void
			registerConnection( common::HeadId headId, common::Connection connection )
			{
				QObject::connect( connection.getReadSocket().get(), &QLocalSocket::disconnected,
				[this, headId] ()
				{
					if( listeners.has( headId ) )
					{
						listeners.get( headId ).goDown();
					}

					if( listeners.getNumberOfListening() == 0 )
					{
						app.quit();
					}
				});

				listeners.add( headId, rod::create< HeadListener >( root, common::HeadId( headId ), std::move( connection ), reduceService ) );
				listeners.get( headId ).listen();
			}
		};


		template< typename Reducibles >
		struct DefineReductions;

		template< typename... Reducible >
		struct DefineReductions< rod::TypeList< Reducible... > >
		{
		private:
			template< typename R >
			struct GetType
			{
				using r = typename R::Type;
			};


		public:
			using r = Reductions< typename GetType< Reducible >::r... >;
		};


		template< typename Context >
		struct DefineContextual
		{
		private:
			using RootDescriptor = typename Context::template FindRegisteredType<
										::heads::annotation::IsRootDescriptor >::r::Head::r;

			template< typename Reducibles >
			struct Define;

			template< typename... ReducibleType >
			struct Define< Reducibles< ReducibleType... > >
			{
				using r = rod::Contextual<
					Context,
					rod::AsSingleton< RootServer >,
					rod::AsSingleton< common::QueryIdProvider >,
					rod::AsSingleton< common::RequestPool >,
					Reducible< ReducibleType >... >;
			};

		public:
			using r = typename Define< typename RootDescriptor::Reducibles >::r;
		};
	}

	template< typename Context >
	class Root:
		public rootDetail::DefineContextual< Context >::r
	{
	private:
		using This = Root< Context >;


	public:
		using Listeners = root::HeadListeners<
								typename rod::Bind< This, HeadListener >
												::template Inject< common::HeadId, common::Connection, HeadReduceService< This >& >::r
												::r >;
		using Reductions = typename rootDetail::DefineReductions<
										typename This::template FindRegisteredType<
													annotation::IsReducibleDescriptor >::r >::r;


	private:
		RootServer&		rootServer = ROD_Resolve( RootServer& );
		Listeners		listeners;
		Reductions		reductions;


	public:
		ROD_Contextual_Constructor( Root );


		Listeners&
		getListeners()
		{
			return listeners;
		}

		Reductions&
		getReductions()
		{
			return reductions;
		}

		void
		enter()
		{
			auto&	app = rod::resolve< QApplication& >( this );
			auto	headReduceService = HeadReduceService< This >( this );
			auto	connectionPreRegister = rod::create< ConnectionPreRegister >( this, createHeadRegisterer( app, listeners, headReduceService ) );

			QMetaObject::Connection c = rootServer.onHeadConnected(
			[this, &connectionPreRegister] ( common::Socket&& headReadSocket )
			{
				connectionPreRegister.preRegisterConnection( std::move( headReadSocket ) );
			});

			app.exec();
			QObject::disconnect( c );
		}


	private:
		template< typename Container, typename ReduceService >
		rootDetail::HeadConnectionRegisterer< This, Container, ReduceService >
		createHeadRegisterer( QApplication& app, Container& container, ReduceService& reduceService )
		{
			return rootDetail::HeadConnectionRegisterer< This, Container, ReduceService >( this, app, container, reduceService );
		}
	};

}}
