#pragma once


#include <list>
#include <map>
#include <utility>

#include <rod/Extend.hpp>
#include <rod/Injected.hpp>
#include <rod/Resolve.hpp>
#include <rod/Singleton.hpp>
#include <rod/Within.hpp>

#include <QMetaObject>
#include <QObject>
#include <QString>

#include <heads/Reducibles.hpp>
#include <heads/common/Connection.hpp>
#include <heads/common/HeadId.hpp>
#include <heads/common/MessageDispatcher.hpp>
#include <heads/common/ServerNaming.hpp>
#include <heads/common/Socket.hpp>
#include <heads/root/HeadIteratorService.hpp>
#include <heads/root/HeadListener.hpp>
#include <heads/root/HeadRegisterer.hpp>
#include <heads/root/Reductions.hpp>
#include <heads/root/RegisterMessage.hpp>




namespace heads {
namespace root
{

	namespace detail
	{

		template< typename Context >
		struct DefineHeadListenersContext
		{
		private:
			using RootDescriptor = typename rod::Find<
										Context,
										rod::match::Annotation< ::heads::annotation::IsRootDescriptor >
									>::r::Head::r;


			template< typename Reducibles >
			struct Define;

			template< typename... Reducible >
			struct Define< Reducibles< Reducible... > >
			{
				using r = typename rod::Extend< Context >
							::template With<
								rod::Singleton< Reductions< Reducible... > > >::r;
			};

		public:
			using r = typename Define< typename RootDescriptor::Reducibles >::r;
		};
		
	}

	template< typename Context >
	class HeadListeners
	{
	private:
		using This = HeadListeners< Context >;

		typename detail::DefineHeadListenersContext< Context >::r	headListenersContext;

		HeadIteratorService< This >	headIteratorService;
		typename rod::Extend< decltype( headListenersContext ) >
			::template With<
				rod::Injected< decltype( headIteratorService )& > >::r	contextWithServices;


		struct HeadConnection
		{
			common::Connection		connection;
			QMetaObject::Connection	registrationConnection;
			QMetaObject::Connection	onConnectedConnection;
			QMetaObject::Connection	onDisconnectedConnection;

			inline
			~HeadConnection()
			{
				QObject::disconnect( registrationConnection );
				QObject::disconnect( onConnectedConnection );
				QObject::disconnect( onDisconnectedConnection );
			}
		};

		using ConnectionList = std::list< HeadConnection >;
		using Listeners = std::map< QString, HeadListener< decltype( contextWithServices ) > >;

		ConnectionList	connections;
		Listeners		listeners;


	public:
		HeadListeners( Context& context ):
		  headListenersContext( context ),
		  headIteratorService( *this ),
		  contextWithServices( headListenersContext, headIteratorService )
		{}

		void
		incomingHeadConnection( common::Socket headReadSocket )
		{
			connections.emplace_front();

			typename ConnectionList::iterator	headConnectionIt = connections.begin();
			HeadConnection&						headConnection = connections.front();
			headConnection.connection.setReadSocket( std::move( headReadSocket ) );


			headConnection.registrationConnection = QObject::connect(
														&headConnection.connection,
														&common::Connection::messageReceived,
			[=] ( const common::Message& message )
			{
				rod::within<
					RegisterMessage >(
					headListenersContext,
					ListenersHeadRegisterer( *this, headConnectionIt ),
				PerformRegistration( message ) );
			});
		}

		Listeners&
		getListeners()
		{
			return listeners;
		}


	private:
		struct PerformRegistration
		{
		private:
			const common::Message&	message;


		public:
			PerformRegistration( const common::Message& message ):
			  message( message )
			{}

			template< typename Ctx >
			void
			operator () ( Ctx& context )
			{
				typename common::CreateMessageDispatcher< Ctx >::r().dispatch( message.type, context, message );
			}
		};


		class ListenersHeadRegisterer:
			public HeadRegisterer
		{
		private:
			HeadListeners< Context >&			parent;
			typename ConnectionList::iterator	headConnectionIt;


		public:
			ListenersHeadRegisterer(
				HeadListeners< Context >& parent,
				typename ConnectionList::iterator headConnectionIt ):
			  parent( parent ),
			  headConnectionIt( headConnectionIt )
			{}


			virtual
			void
			registerHeadWithId( common::HeadId headId )
			{
				parent.registerHead( headConnectionIt, std::move( headId ) );
			}
		};


		void
		registerHead( typename ConnectionList::iterator headConnectionIt, common::HeadId headId )
		{
			QObject::disconnect( headConnectionIt->registrationConnection );

			listeners.emplace(
				headId.id,
				HeadListener< decltype( contextWithServices ) >(
					contextWithServices,
					headConnectionIt->connection,
					common::HeadId( headId ) ) );

			headConnectionIt->connection.setWriteSocket( common::Socket( new QLocalSocket() ) );
			headConnectionIt->onConnectedConnection = QObject::connect(
				headConnectionIt->connection.getWriteSocket().get(),
				&QLocalSocket::connected,
			[=] ()
			{
				QObject::disconnect( headConnectionIt->onConnectedConnection );
				// TODO enable messageReceived() events on connection?
			});
			headConnectionIt->connection.getWriteSocket()->connectToServer(
				rod::resolve< common::ServerNaming& >( headListenersContext ).headName( headId ), QIODevice::WriteOnly );

			headConnectionIt->onDisconnectedConnection = QObject::connect(
				headConnectionIt->connection.getReadSocket().get(),
				&QLocalSocket::disconnected,
			[=] ()
			{
				shutDownListener( headId );
			});
		}

		void
		shutDownListener( const common::HeadId& headId )
		{
			typename Listeners::iterator	headIt = listeners.find( headId.id );
			if( headIt != listeners.end() )
			{
				headIt->second.goDown();
			}

			if( getNumberOfListening() == 0 )
			{
				rod::resolve< QApplication& >( headListenersContext ).quit();
			}
		}

		int
		getNumberOfListening() const
		{
			int	listening = 0;

			for( auto& listener: listeners )
			{
				if( listener.second.getStage() == ListenerStage::Listening )
				{
					++listening;
				}
			}

			return listening;
		}

	};

}}
