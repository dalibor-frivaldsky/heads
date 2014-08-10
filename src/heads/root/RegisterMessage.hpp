#pragma once


#include <rod/Contextual.hpp>

#include <QObject>
#include <QString>

#include <heads/annotation/MessageController.hpp>
#include <heads/common/HeadId.hpp>
#include <heads/common/ServerNaming.hpp>
#include <heads/common/Socket.hpp>
#include <heads/root/ConnectionRegisterer.hpp>
#include <heads/root/UnpairedSockets.hpp>




namespace heads {
namespace root
{

	template< typename Context >
	class RegisterMessage:
		public rod::Contextual< Context >
	{
	private:
		ConnectionRegisterer&		connectionRegisterer = ROD_Resolve( ConnectionRegisterer& );
		common::ServerNaming&		serverNaming = ROD_Resolve( common::ServerNaming& );
		UnpairedSockets&			unpairedSockets = ROD_Resolve( UnpairedSockets& );
		UnpairedSockets::iterator&	unpairedSocketIt = ROD_Resolve( UnpairedSockets::iterator& );



	public:
		ROD_Contextual_Constructor( RegisterMessage );


		using MessageController = ::heads::annotation::MessageController< common::HeadId >;

		static
		QString
		endpoint()
		{
			return "head/register";
		}

		void
		operator () ( common::HeadId headId )
		{
			// TODO figure out how to get rid of the raw pointer
			QLocalSocket*	rawHeadWriteSocket = new QLocalSocket();

			QObject::connect( rawHeadWriteSocket, &QLocalSocket::connected,
			[=] ()
			{
				this->connectionRegisterer.registerConnection(
						headId,
						common::Connection(
							std::move( unpairedSocketIt->readSocket ),
							common::Socket( rawHeadWriteSocket ),
							std::move( unpairedSocketIt->protocolReader ) ) );

				QObject::disconnect( unpairedSocketIt->connection );
				unpairedSockets.removeUnpairedReadSocket( unpairedSocketIt );
			});

			rawHeadWriteSocket->connectToServer( serverNaming.headName( headId ), QIODevice::WriteOnly );
		}

	};

}}
