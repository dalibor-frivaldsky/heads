#include "HeadServer.hpp"




namespace heads {
namespace head
{

	HeadServer::HeadServer( common::ServerNaming& serverNaming ):
	  serverNaming( serverNaming )
	{
		QLocalServer::removeServer( serverNaming.headName( id ) );
		server.listen( serverNaming.headName( id ) );
	}

	HeadServer::~HeadServer()
	{
		server.close();
		QLocalServer::removeServer( serverNaming.headName( id ) );
	}

	common::HeadId
	HeadServer::getId() const
	{
		return id;
	}

	QMetaObject::Connection
	HeadServer::onRootConnected( RootConnectedCallback&& callback )
	{
		return QObject::connect( &server, &QLocalServer::newConnection, [=] ()
		{
			callback( common::Socket( server.nextPendingConnection() ) );
		});
	}

}}
