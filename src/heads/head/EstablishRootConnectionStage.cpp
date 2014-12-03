#include "EstablishRootConnectionStage.hpp"


#include <utility>

#include <QProcess>
#include <QThread>

#include <rod/log/Level.hpp>
using namespace rod::log::level;

#include <heads/common/Options.hpp>




namespace heads {
namespace head {

	EstablishRootConnectionStage::EstablishRootConnectionStage(
		rod::log::Logger< This > log,
		common::Connection& connection,
		common::ServerNaming& serverNaming,
		StageControl< bool >& stageControl ):
	  QObject(),
	  log( std::move( log ) ),
	  connection( connection ),
	  serverNaming( serverNaming ),
	  stageControl( stageControl )
	{}
    
    EstablishRootConnectionStage::EstablishRootConnectionStage( EstablishRootConnectionStage&& other ):
      QObject(),
      log( std::move( other.log ) ),
      connection( other.connection ),
      serverNaming( other.serverNaming ),
      stageControl( other.stageControl ),
      serverStarted( other.serverStarted )
    {}

	void
	EstablishRootConnectionStage::connectToRootServer()
	{
		log.debug( "Establishing write connection to root" );
		connection.getWriteSocket()->connectToServer( serverNaming.rootName(), QIODevice::WriteOnly );
	}

	void
	EstablishRootConnectionStage::onConnected()
	{
		log.info( "Write connection to root established" );
		stageControl.done( true );
	}

	void
	EstablishRootConnectionStage::onError( QLocalSocket::LocalSocketError socketError )
	{
		switch( socketError )
		{
			case QLocalSocket::ServerNotFoundError:
				log.debug( "Root not found" );

				if( serverStarted == false )
				{
					log.debug( "Creating root process" );

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
				log.fatal( "Write connection to root could not be established" );

				stageControl.done( false );
				break;
		}
	}
	
}}