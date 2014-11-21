#pragma once


#include <functional>

#include <rod/annotation/Requires.hpp>

#include <QLocalServer>
#include <QMetaObject>
#include <QUuid>

#include <heads/common/HeadId.hpp>
#include <heads/common/ServerNaming.hpp>
#include <heads/common/Socket.hpp>




namespace heads {
namespace head
{

	class HeadServer
	{
	public:
		using RootConnectedCallback = std::function< void( common::Socket ) >;


	private:
		common::ServerNaming&	serverNaming;

		common::HeadId	id = common::HeadId( QUuid::createUuid().toString() );
		QLocalServer	server;


	public:
		using Requires = rod::annotation::Requires< common::ServerNaming& >;
		HeadServer( common::ServerNaming& serverNaming );

		~HeadServer();

		common::HeadId	getId() const;

		QMetaObject::Connection	onRootConnected( RootConnectedCallback&& callback );
	};

}}
