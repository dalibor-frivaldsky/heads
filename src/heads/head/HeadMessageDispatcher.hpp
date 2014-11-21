#pragma once


#include <rod/Resolve.hpp>

#include <QMetaObject>
#include <QObject>

#include <heads/common/Connection.hpp>
#include <heads/common/Message.hpp>
#include <heads/common/MessageDispatcher.hpp>
#include <heads/common/RequestPool.hpp>
#include <heads/head/HeadQueryId.hpp>




namespace heads {
namespace head
{

	template< typename Context >
	class HeadMessageDispatcher
	{
	private:
		typename common::CreateMessageDispatcher< Context >::r	dispatcher;

		Context&				context;
		common::Connection&		connection;
		common::RequestPool&	requestPool;

		QMetaObject::Connection		onNewMessageConnection;


	public:
		HeadMessageDispatcher( Context& context ):
		  context( context ),
		  connection( rod::resolve< common::Connection& >( context ) ),
		  requestPool( rod::resolve< common::RequestPool& >( context ) )
		{}

		~HeadMessageDispatcher()
		{
			QObject::disconnect( onNewMessageConnection );
		}

		void
		start()
		{
			onNewMessageConnection = QObject::connect(
				&connection,
				&common::Connection::messageReceived,
			[this] ( const common::Message& message )
			{
				if( message.type.isEmpty() )
				{
					requestPool.processMessage( message );
				}
				else
				{
					dispatcher.dispatch( message.type, context, message, HeadQueryId { message.id } );
				}
			});
		}
	};
	
}}
