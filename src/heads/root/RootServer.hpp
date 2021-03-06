#pragma once


#include <functional>
#include <memory>

#include <QLocalServer>
#include <QMetaObject>

#include <rod/annotation/Requires.hpp>

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
		using Requires = rod::annotation::Requires< common::ServerNaming& >;
		RootServer( common::ServerNaming& serverNaming );

		~RootServer();

		void	listen();

		QMetaObject::Connection	onHeadConnected( HeadConnectedCallback&& callback );
	};

}}
