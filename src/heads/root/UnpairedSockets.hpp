#pragma once


#include <list>

#include <QMetaObject>

#include <heads/common/Socket.hpp>
#include <heads/common/ProtocolReader.hpp>




namespace heads {
namespace root
{

	namespace unpairedSockets
	{
	
		struct UnpairedSocket
		{
			common::Socket				readSocket;
			common::ProtocolReader		protocolReader;
			QMetaObject::Connection		connection;

			UnpairedSocket( common::Socket readSocket );
			UnpairedSocket( UnpairedSocket&& other );
		};
	
	}


	class UnpairedSockets
	{
	private:
		using UnpairedSocketsSet = std::list< unpairedSockets::UnpairedSocket >;

		UnpairedSocketsSet	unpairedReadSockets;


	public:
		using iterator = UnpairedSocketsSet::iterator;


		iterator	addReadSocket( common::Socket readDevice );
		void		removeUnpairedReadSocket( iterator it );
	};

}}
