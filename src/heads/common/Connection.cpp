#include "Connection.hpp"


#include <utility>




namespace heads {
namespace common
{

	Connection::Connection()
	{}

	Connection::Connection( Socket readSocket, Socket writeSocket, ProtocolReader protocolReader ):
	  readSocket( std::move( readSocket ) ),
	  writeSocket( std::move( writeSocket ) ),
	  protocolReader( std::move( protocolReader ) )
	{}

	Connection::Connection( Connection&& other ):
	  readSocket( std::move( other.readSocket ) ),
	  writeSocket( std::move( other.writeSocket ) ),
	  protocolReader( std::move( other.protocolReader ) )
	{}

	Connection::~Connection()
	{}

	const Socket&
	Connection::getReadSocket() const
	{
		return readSocket;
	}

	void
	Connection::setReadSocket( Socket&& readSocket )
	{
		this->readSocket = std::move( readSocket );
	}

	const Socket&
	Connection::getWriteSocket() const
	{
		return writeSocket;
	}

	void
	Connection::setWriteSocket( Socket&& writeSocket )
	{
		this->writeSocket = std::move( writeSocket );
	}

	ProtocolReader&
	Connection::getProtocolReader()
	{
		return protocolReader;
	}

}}
