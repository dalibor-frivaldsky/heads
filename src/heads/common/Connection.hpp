#pragma once


#include "heads/common/ProtocolReader.hpp"
#include "heads/common/Socket.hpp"




namespace heads {
namespace common
{

	class Connection
	{
	private:
		Socket			readSocket;
		Socket			writeSocket;
		ProtocolReader	protocolReader;


	public:
		Connection();
		Connection( Socket readSocket, Socket writeSocket, ProtocolReader protocolReader );
		Connection( Connection&& other );
		~Connection();

		Connection( const Connection& ) = delete;

		const Socket&		getReadSocket() const;
		void				setReadSocket( Socket&& readSocket );
		const Socket&		getWriteSocket() const;
		void				setWriteSocket( Socket&& writeSocket );
		ProtocolReader&		getProtocolReader();
	};

}}
