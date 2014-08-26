#include "RootServer.hpp"


#include <QObject>




namespace heads {
namespace root
{

	RootServer::RootServer( common::ServerNaming& serverNaming ):
	  serverNaming( serverNaming )
	{}

	RootServer::~RootServer()
	{
		rootServer.close();
		QLocalServer::removeServer( serverNaming.rootName() );
	}

	void
	RootServer::listen()
	{
		rootServer.listen( serverNaming.rootName() );
	}

	QMetaObject::Connection
	RootServer::onHeadConnected( HeadConnectedCallback&& callback )
	{
		return QObject::connect( &rootServer, &QLocalServer::newConnection, [=] ()
		{
			callback( common::Socket( rootServer.nextPendingConnection() ) );
		});
	}

}}
