#pragma once


#include <functional>
#include <memory>

#include <QLocalServer>
#include <QMetaObject>

#include <rod/annotation/ConstructWith.hpp>

#include <heads/common/ServerNaming.hpp>
#include <heads/common/Socket.hpp>




namespace heads {
namespace root
{

	class RootServer
	{
	public:
		using HeadConnectedCallback = std::function< void( common::Socket ) >;


	private:
		common::ServerNaming&	serverNaming;

		QLocalServer	rootServer;


	public:
		using ConstructWith = rod::annotation::ConstructWith< common::ServerNaming& >;
		RootServer( common::ServerNaming& serverNaming );

		~RootServer();

		QMetaObject::Connection	onHeadConnected( HeadConnectedCallback&& callback );
	};

}}
