#pragma once


#include <rod/annotation/Requires.hpp>
#include <rod/match/Annotation.hpp>

#include <rod/factory/Create.hpp>

#include <rod/log/Logger.hpp>

#include <QObject>

#include <heads/common/Connection.hpp>
#include <heads/common/ServerNaming.hpp>
#include <heads/common/Socket.hpp>
#include <heads/head/StageControl.hpp>




namespace heads {
namespace head {

	class EstablishRootConnectionStage:
		public QObject
	{
		Q_OBJECT


	private:
		using This = EstablishRootConnectionStage;

		rod::log::Logger< This >	log;
		common::Connection&			connection;
		common::ServerNaming&		serverNaming;
		StageControl< bool >&		stageControl;

		bool	serverStarted = false;


	public:
		using Requires = rod::annotation::Requires<
							rod::log::Logger< This >,
							common::Connection&,
							common::ServerNaming&,
							StageControl< bool >& >;

		EstablishRootConnectionStage(
			rod::log::Logger< This > log,
			common::Connection& connection,
			common::ServerNaming& serverNaming,
			StageControl< bool >& stageControl );
        
        EstablishRootConnectionStage( EstablishRootConnectionStage&& other );

		template< typename Context >
		void
		execute( Context& )
		{
			connection.setWriteSocket( common::Socket( new QLocalSocket() ) );

			QObject::connect(
				connection.getWriteSocket().get(),
				&QLocalSocket::connected,
				this,
				&EstablishRootConnectionStage::onConnected,
				Qt::QueuedConnection );

			QObject::connect(
				connection.getWriteSocket().get(),
				SIGNAL( error( QLocalSocket::LocalSocketError ) ),
				this,
				SLOT( onError( QLocalSocket::LocalSocketError ) ),
				Qt::QueuedConnection );

			connectToRootServer();
		}


	private:
		void	connectToRootServer();


	private slots:
		void	onConnected();
		void	onError( QLocalSocket::LocalSocketError socketError );
	};
	
}}