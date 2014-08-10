#pragma once


#include <utility>

#include <rod/AsContextual.hpp>
#include <rod/AsSingleton.hpp>
#include <rod/Contextual.hpp>

#include <QBuffer>
#include <QDataStream>
#include <QMetaObject>

#include <heads/Bootstrapper.hpp>
#include <heads/Inform.hpp>
#include <heads/MessageControllers.hpp>
#include <heads/annotation/HeadDescriptor.hpp>
#include <heads/common/HeadId.hpp>
#include <heads/common/Message.hpp>
#include <heads/common/MessageDispatcher.hpp>
#include <heads/common/QueryIdProvider.hpp>
#include <heads/common/RequestPool.hpp>
#include <heads/head/HeadServer.hpp>
#include <heads/head/RootConnection.hpp>




namespace heads {
namespace head
{

	namespace headDetail
	{
	
		template< typename Context >
		struct DefineContextual
		{
		private:
			using HeadDescriptor = typename Context::template FindRegisteredType<
										annotation::IsHeadDescriptor >::r::Head::r;

			template< typename MessageControllers >
			struct Define;

			template< typename... MessageController >
			struct Define< MessageControllers< MessageController... > >
			{
				using r = rod::Contextual<
					Context,
					rod::AsSingleton< common::QueryIdProvider >,
					rod::AsSingleton< common::RequestPool >,
					rod::AsSingleton< head::HeadServer >,
					rod::AsSingleton< head::RootConnection >,
					MessageController... >;
			};

		public:
			using r = typename Define< typename HeadDescriptor::MessageControllers >::r;
		};
	
	}


	template< typename Context >
	class Head:
		public headDetail::DefineContextual< Context >::r
	{
	private:
		using This = Head< Context >;


		HeadServer&				headServer = ROD_Resolve( HeadServer& );
		common::Connection&		connection = ROD_Resolve( common::Connection& );
		common::RequestPool&	requestPool = ROD_Resolve( common::RequestPool& );


	public:
		ROD_Contextual_Constructor( Head );


		void
		setRootSocket( common::Socket socket )
		{
			rod::resolve< RootConnection& >( this ).setWriteSocket( std::move( socket ) );
		}

		void
		enter()
		{
			registerToRoot();
			if( bootstrap() )
			{
				launchLogic();
			}
		}


	private:
		void
		registerToRoot()
		{
			QEventLoop					eventLoop;
			QMetaObject::Connection		c;

			c = headServer.onRootConnected(
			[=, &eventLoop] ( common::Socket&& rootReadSocket )
			{
				rod::resolve< RootConnection& >( this ).setReadSocket( std::move( rootReadSocket ) );
					
				eventLoop.quit();
			});

			inform( this, topic( "head/register", headServer.getId() ) );

			eventLoop.exec();
			QObject::disconnect( c );
		}

		bool
		bootstrap()
		{
			QEventLoop				eventLoop;
			Bootstrapper			bootstrapper( eventLoop );
			auto					dispatcher = common::messageDispatcher( this );
			QMetaObject::Connection	c = QObject::connect( connection.getReadSocket().get(), &QIODevice::readyRead,
			[this, &dispatcher] ()
			{
				connection.getProtocolReader().readFrom( *connection.getReadSocket() );

				while( connection.getProtocolReader().availableMessages() )
				{
					common::Message	m( connection.getProtocolReader().get() );

					if( m.type.isEmpty() )
					{
						requestPool.processMessage( std::move( m ) );
					}
					else
					{
						common::QueryId	id { m.id };
						dispatcher.dispatch( m.type, std::move( m ), std::move( id ) );
					}
				}
			});

			using	headBootstrapContextual = typename This::template FindRegisteredType<
													annotation::IsHeadDescriptor >::r::Head::r::Bootstrap;
			auto	headBootstrap = rod::create< headBootstrapContextual::template Contextual >( this, bootstrapper );

			headBootstrap.bootstrap();

			eventLoop.exec();
			QObject::disconnect( c );

			return bootstrapper.shouldProceed();
		}

		void
		launchLogic()
		{
			using	headLogicContextual = typename This::template FindRegisteredType<
													annotation::IsHeadDescriptor >::r::Head::r::Logic;
			auto	headLogic = rod::create< headLogicContextual::template Contextual >( this );

			headLogic.build();

			auto					dispatcher = common::messageDispatcher( this );
			QMetaObject::Connection	c = QObject::connect( connection.getReadSocket().get(), &QIODevice::readyRead,
			[this, &dispatcher] ()
			{
				connection.getProtocolReader().readFrom( *connection.getReadSocket() );

				while( connection.getProtocolReader().availableMessages() )
				{
					common::Message	m( connection.getProtocolReader().get() );

					if( m.type.isEmpty() )
					{
						requestPool.processMessage( std::move( m ) );
					}
					else
					{
						common::QueryId	id { m.id };
						dispatcher.dispatch( m.type, std::move( m ), std::move( id ) );
					}
				}
			});

			rod::resolve< QApplication& >( this ).exec();
			QObject::disconnect( c );
		}
	};

}}
