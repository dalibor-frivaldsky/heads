#pragma once


#include <utility>

#include <rod/Extend.hpp>
#include <rod/Find.hpp>
#include <rod/Injected.hpp>
#include <rod/Singleton.hpp>
#include <rod/match/Annotation.hpp>
#include <rod/match/Component.hpp>

#include <QMetaObject>
#include <QObject>

#include <heads/MessageControllers.hpp>
#include <heads/annotation/RootDescriptor.hpp>
#include <heads/common/Connection.hpp>
#include <heads/common/HeadId.hpp>
#include <heads/common/Message.hpp>
#include <heads/common/MessageDispatcher.hpp>
#include <heads/common/QueryId.hpp>
#include <heads/common/QueryIdProvider.hpp>
#include <heads/common/QueryService.hpp>
#include <heads/common/RequestPool.hpp>
#include <heads/root/HeadReduceService.hpp>
#include <heads/root/annotation/ReductionPool.hpp>




namespace heads {
namespace root
{

	namespace detail
	{

		template< typename Context >
		struct DefineListenerContext
		{
		private:
			using RootDescriptor = typename rod::Find<
										Context,
										rod::match::Annotation< ::heads::annotation::IsRootDescriptor >
									>::r::Head::r;

			template< typename MessageControllers >
			struct Define;

			template< typename... MessageController >
			struct Define< MessageControllers< MessageController... > >
			{
				using r = typename rod::Extend< Context >
							::template With<
								MessageController...,
								rod::Injected< common::Connection& >,
								rod::Injected< common::HeadId > >::r;
			};

		public:
			using r = typename Define< typename RootDescriptor::MessageControllers >::r;
		};

	}


	enum ListenerStage
	{
		Initializing,
		Listening,
		Down
	};

	template< typename Context >
	class HeadListener
	{
	private:
		typename detail::DefineListenerContext< Context >::r		listenerContext;

		HeadReduceService< decltype( listenerContext ) >	headReduceService;
		common::QueryService< decltype( listenerContext ) >		queryService;
		typename rod::Extend< decltype( listenerContext ) >
			::template With<
				rod::Injected< decltype( headReduceService )& >,
				rod::Injected< decltype( queryService )& > >::r	listenerContextWithServices;


		ListenerStage				stage = ListenerStage::Initializing;
		QMetaObject::Connection		onMessageReceivedConnection;

		common::Connection&		connection;


	public:
		HeadListener( Context& context, common::Connection& connection, common::HeadId headId ):
		  listenerContext( context, connection, std::move( headId ) ),
		  headReduceService( listenerContext ),
		  queryService( listenerContext ),
		  listenerContextWithServices( listenerContext, headReduceService, queryService ),
		  connection( connection )
		{
			listen();
		}

		HeadListener( const HeadListener< Context >& ) = delete;

		HeadListener( HeadListener< Context >&& other ):
		  listenerContext( std::move( other.listenerContext ) ),
		  headReduceService( listenerContext ),
		  queryService( listenerContext ),
		  listenerContextWithServices( listenerContext, headReduceService, queryService ),
		  stage( other.stage ),
		  connection( other.connection )
		{
			listen();
		}

		~HeadListener()
		{
			QObject::disconnect( onMessageReceivedConnection );
		}

		decltype(listenerContextWithServices)&
		getContext()
		{
			return listenerContextWithServices;
		}

		common::HeadId
		getId()
		{
			return rod::resolve< common::HeadId& >( this );
		}

		ListenerStage
		getStage() const
		{
			return stage;
		}

		void
		listen()
		{
			onMessageReceivedConnection = QObject::connect( &connection, &common::Connection::messageReceived,
			[this] ( const common::Message& message )
			{
				using	ReductionPool =	typename rod::Find<
											Context,
											rod::match::Component<
												rod::match::Annotation< root::annotation::IsReductionPool >
											>
										>::r::Head::r;

                using       Dispatcher = typename common::CreateMessageDispatcher<
                                            decltype( listenerContext ) >::r;
				Dispatcher  dispatcher;
				
				if( message.type.isEmpty() )
				{
					rod::resolve< common::RequestPool& >( listenerContext ).processMessage( message );
					rod::resolve< ReductionPool& >( listenerContext ).check();
				}
				else
				{
					dispatcher.dispatch(
						message.type,
						listenerContextWithServices,
						message,
						common::QueryId { message.id } );
				}
			});

			stage = ListenerStage::Listening;
		}

		void
		goDown()
		{
			QObject::disconnect( onMessageReceivedConnection );
			stage = ListenerStage::Down;
		}
	};

}}
