#include "EstablishRootConnectionStage.hpp"


#include <QProcess>
#include <QThread>

#include <heads/common/Options.hpp>




namespace heads {
namespace head {

	EstablishRootConnectionStage::EstablishRootConnectionStage(
		common::Connection& connection,
		common::ServerNaming& serverNaming,
		StageControl< bool >& stageControl ):
	  QObject(),
	  connection( connection ),
	  serverNaming( serverNaming ),
	  stageControl( stageControl )
	{}

	void
	EstablishRootConnectionStage::connectToRootServer()
	{
		connection.getWriteSocket()->connectToServer( QIODevice::WriteOnly );
	}

	void
	EstablishRootConnectionStage::onConnected()
	{
		stageControl.done( true );
	}

	void
	EstablishRootConnectionStage::onError( QLocalSocket::LocalSocketError socketError )
	{
		switch( socketError )
		{
			case QLocalSocket::ServerNotFoundError:
				if( serverStarted == false )
				{
					QProcess::startDetached(
						QCoreApplication::arguments()[ 0 ],
						QStringList() << common::Options::startAsRoot() );

					serverStarted = true;
				}

				QThread::msleep( 100 );
				connectToRootServer();

				break;

			default:
				// TODO some general error handling
				stageControl.done( false );
				break;
		}
	}
	
}}