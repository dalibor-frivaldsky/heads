#include "ProtocolReader.hpp"


#include <utility>

#include <QBuffer>
#include <QDataStream>




namespace heads {
namespace common
{

	ProtocolReader::ProtocolReader()
	{}

	ProtocolReader::ProtocolReader( ProtocolReader&& other ):
	  nextMessageSize( other.nextMessageSize ),
	  remainingChunkSize( other.remainingChunkSize ),
	  accumulator( std::move( other.accumulator ) ),
	  readMessages( std::move( other.readMessages ) )
	{}

	ProtocolReader::~ProtocolReader()
	{}

	ProtocolReader&
	ProtocolReader::operator = ( ProtocolReader&& other )
	{
		nextMessageSize = other.nextMessageSize;
		remainingChunkSize = other.remainingChunkSize;
		accumulator = std::move( other.accumulator );
		readMessages = std::move( other.readMessages );

		return *this;
	}

	void
	ProtocolReader::readFrom( QIODevice& inputDevice )
	{
		while( inputDevice.bytesAvailable() )
		{
			QByteArray nextData = inputDevice.read( remainingChunkSize );
			accumulator.append( nextData );
			remainingChunkSize -= nextData.size();

			if( remainingChunkSize == 0 )
			{
				QBuffer		buffer( &accumulator );
				QDataStream	stream( &buffer );

				buffer.open( QIODevice::ReadOnly );
				if( nextMessageSize == -1 )
				{
					stream >> nextMessageSize;
					remainingChunkSize = nextMessageSize;
				}
				else
				{
					Message	m;
					stream >> m;

					readMessages.push_back( std::move( m ) );

					nextMessageSize = -1;
					remainingChunkSize = sizeof( int );
				}
				buffer.close();
				accumulator.clear();
			}
		}
	}

	bool
	ProtocolReader::availableMessages() const
	{
		return readMessages.size();
	}

	Message
	ProtocolReader::get()
	{
		Message	m( std::move( readMessages.front() ) );
		readMessages.pop_front();

		return std::move( m );
	}

}}
