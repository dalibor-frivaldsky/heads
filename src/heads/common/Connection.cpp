#include "Connection.hpp"


#include <utility>

#include <heads/common/ProtocolWriter.hpp>




namespace heads {
namespace common
{

	Connection::Connection():
	  QObject()
	{}

	Connection::Connection( Socket readSocket, Socket writeSocket, ProtocolReader protocolReader ):
	  QObject(),
	  readSocket( std::move( readSocket ) ),
	  writeSocket( std::move( writeSocket ) ),
	  protocolReader( std::move( protocolReader ) )
	{}

	Connection::Connection( Connection&& other ):
	  QObject(),
	  readSocket( std::move( other.readSocket ) ),
	  writeSocket( std::move( other.writeSocket ) ),
	  protocolReader( std::move( other.protocolReader ) )
	{}

	Connection::~Connection()
	{
		if( readSocket && readSocket->isValid() )
		{
			readSocket->close();
		}

		if( writeSocket )
		{
			writeSocket->close();
		}
	}

	const Socket&
	Connection::getReadSocket() const
	{
		return readSocket;
	}

	void
	Connection::setReadSocket( Socket&& readSocket )
	{
		this->readSocket = std::move( readSocket );

		QObject::connect( this->readSocket.get(), &QIODevice::readyRead,
						  this, &Connection::onReadyRead );
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

	void
	Connection::sendMessage( const Message& message )
	{
		ProtocolWriter::write( *writeSocket, message );
	}

	void
	Connection::onReadyRead()
	{
		protocolReader.readFrom( *readSocket );

		while( protocolReader.availableMessages() )
		{
			common::Message	message( protocolReader.get() );

			emit messageReceived( message );
		}
	}

}}
