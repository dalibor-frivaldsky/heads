#pragma once


#include <vector>
#include <utility>

#include <rod/AsContextual.hpp>
#include <rod/AsSingleton.hpp>
#include <rod/Contextual.hpp>

#include <QMetaObject>
#include <QObject>

#include <heads/common/Message.hpp>
#include <heads/common/MessageDispatcher.hpp>
#include <heads/common/Socket.hpp>
#include <heads/root/RegisterMessage.hpp>
#include <heads/root/UnpairedSockets.hpp>




namespace heads {
namespace root
{

	template< typename Context >
	class ConnectionPreRegister:
		public rod::Contextual<
				Context,
				rod::AsSingleton< UnpairedSockets >,
				rod::AsContextual< RegisterMessage > >
	{
	private:
		using This = ConnectionPreRegister< Context >;


		UnpairedSockets&	unpairedSockets = ROD_Resolve( UnpairedSockets& );

		decltype( common::messageDispatcher( &std::declval< This >() ) )	dispatcher = common::messageDispatcher( this );


	public:
		ROD_Contextual_Constructor( ConnectionPreRegister );

		void
		preRegisterConnection( common::Socket readSocket )
		{
			UnpairedSockets::iterator	unpairedSocketIt = unpairedSockets.addReadSocket( std::move( readSocket ) );
			unpairedSocketIt->connection = QObject::connect( unpairedSocketIt->readSocket.get(), &QIODevice::readyRead,
			[=] ()
			{
				unpairedSocketIt->protocolReader.readFrom( *unpairedSocketIt->readSocket );
				if( unpairedSocketIt->protocolReader.availableMessages() )
				{
					common::Message	message( unpairedSocketIt->protocolReader.get() );
					dispatcher.dispatch( message.type, std::move( message ), UnpairedSockets::iterator( unpairedSocketIt ) );
				}
			});
		}
	};

}}
