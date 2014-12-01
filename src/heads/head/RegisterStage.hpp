#pragma once


#include <utility>

#include <rod/Resolve.hpp>
#include <rod/With.hpp>
#include <rod/annotation/Requires.hpp>

#include <QMetaObject>
#include <QObject>

#include <heads/Inform.hpp>
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
		Inform					inform;

		QMetaObject::Connection		onRootConnectedConnection;


	public:
		using Requires = rod::annotation::Requires<
							common::Connection&,
							HeadServer&,
							StageControl< void >&,
							Inform >;

		RegisterStage(
			common::Connection& connection,
			HeadServer& headServer,
			StageControl< void >& stageControl,
			Inform inform ):
		  connection( connection ),
		  headServer( headServer ),
		  stageControl( stageControl ),
		  inform( std::move( inform ) )
		{}

		~RegisterStage()
		{
			QObject::disconnect( onRootConnectedConnection );
		}

		template< typename Context >
		void
		execute( Context& )
		{
			onRootConnectedConnection = headServer.onRootConnected(
			[this] ( common::Socket rootReadSocket )
			{
				connection.setReadSocket( std::move( rootReadSocket ) );
				stageControl.done();
			});
			inform( topic( "head/register", headServer.getId() ) );
		}
	};
	
}}