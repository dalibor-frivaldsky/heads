#include "ProtocolWriter.hpp"


#include <QBuffer>
#include <QByteArray>
#include <QDataStream>




namespace heads {
namespace common
{

	void
	ProtocolWriter::write( QIODevice& writeDevice, const Message& message )
	{
		QByteArray	rawMessage;
		QBuffer		buffer( &rawMessage );
		QDataStream	messageStream( &buffer );

		buffer.open( QIODevice::WriteOnly );
		messageStream << message;
		buffer.close();

		QDataStream	socketStream( &writeDevice );
		socketStream << rawMessage.size();
		writeDevice.write( rawMessage );
	}

}}
