#pragma once


#include <utility>

#include <rod/AsContextual.hpp>
#include <rod/Contextual.hpp>

#include <QMetaObject>

#include <heads/MessageControllers.hpp>
#include <heads/annotation/RootDescriptor.hpp>
#include <heads/common/Connection.hpp>
#include <heads/common/Message.hpp>
#include <heads/common/MessageDispatcher.hpp>
#include <heads/common/QueryId.hpp>
#include <heads/common/RequestPool.hpp>
#include <heads/root/annotation/ReduceService.hpp>




namespace heads {
namespace root
{

	namespace headListenerDetail
	{

		template< typename Context >
		struct DefineContextual
		{
		private:
			using RootDescriptor = typename Context::template FindRegisteredType<
										::heads::annotation::IsRootDescriptor >::r::Head::r;

			template< typename MessageControllers >
			struct Define;

			template< typename... MessageController >
			struct Define< MessageControllers< MessageController... > >
			{
				using r = rod::Contextual<
					Context,
					MessageController... >;
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
	class HeadListener:
		public headListenerDetail::DefineContextual< Context >::r
	{
	private:
		using This = HeadListener< Context >;

		ListenerStage				stage = ListenerStage::Initializing;
		QMetaObject::Connection		readyReadConnection;

		common::Connection&		connection = ROD_Resolve( common::Connection& );


	public:
		ROD_Contextual_Constructor( HeadListener );

		HeadListener( This&& other ):
		  This::ContextualBase( std::move( other ) ),
		  connection( rod::resolve< common::Connection& >( this ) ),
		  stage( other.stage ),
		  readyReadConnection( std::move( other.readyReadConnection ) )
		{}

		~HeadListener()
		{
			if( readyReadConnection )
			{
				QObject::disconnect( readyReadConnection );
			}
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
			using ReduceService = typename This::template FindRegisteredType< annotation::IsReduceService >::r::Head::r;

			readyReadConnection = QObject::connect( connection.getReadSocket().get(), &QIODevice::readyRead,
			[this] ()
			{
				connection.getProtocolReader().readFrom( *connection.getReadSocket() );

				while( connection.getProtocolReader().availableMessages() )
				{
					common::Message	m( connection.getProtocolReader().get() );
					auto			dispatcher = common::messageDispatcher( this );
				
					if( m.type.isEmpty() )
					{
						rod::resolve< common::RequestPool& >( this ).processMessage( std::move( m ) );
						rod::resolve< ReduceService& >( this ).checkReductions();
					}
					else
					{
						common::QueryId	id { m.id };
						dispatcher.dispatch( m.type, std::move( m ), std::move( id ) );
					}
				}
			});

			stage = ListenerStage::Listening;
		}

		void
		goDown()
		{
			QObject::disconnect( readyReadConnection );
			stage = ListenerStage::Down;
		}
	};

}}
