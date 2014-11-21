#pragma once


#include <rod/Resolve.hpp>
#include <rod/With.hpp>
#include <rod/annotation/Requires.hpp>

#include <QMetaObject>
#include <QObject>

#include <heads/common/Connection.hpp>
#include <heads/head/HeadServer.hpp>
#include <heads/head/StageControl.hpp>




namespace heads {
namespace head {

	class RegisterStage
	{
	private:
		common::Connection&		connection;
		HeadServer&				headServer;
		StageControl< void >&	stageControl;

		QMetaObject::Connection		onRootConnectedConnection;


	public:
		using Requires = rod::annotation::Requires< common::Connection&, HeadServer&, StageControl< void >& >;

		RegisterStage( common::Connection& connection, HeadServer& headServer, StageControl< void >& stageControl ):
		  connection( connection ),
		  headServer( headServer ),
		  stageControl( stageControl )
		{}

		~RegisterStage()
		{
			QObject::disconnect( onRootConnectedConnection );
		}

		template< typename Context >
		void
		execute( Context& context )
		{
			onRootConnectedConnection = headServer.onRootConnected(
			[this] ( common::Socket rootReadSocket )
			{
				connection.setReadSocket( std::move( rootReadSocket ) );
				stageControl.done();
			});
			inform( context, topic( "head/register", headServer.getId() ) );
		}
	};
	
}}