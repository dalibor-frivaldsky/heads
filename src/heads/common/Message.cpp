#include "Message.hpp"




namespace heads {
namespace common
{

	Message::Message()
	{}

	Message::Message( Message&& other ):
	  type( std::move( other.type ) ),
	  id( std::move( other.id ) ),
	  content( std::move( other.content ) )
	{}

	Message&
	Message::operator = ( Message&& other )
	{
		this->type = std::move( other.type );
		this->id = std::move( other.id );
		this->content = std::move( other.content );

		return *this;
	}

}}


QDataStream&
operator << ( QDataStream& stream, const ::heads::common::Message& message )
{
	stream << message.type;
	stream << message.id;
	stream << message.content;

	return stream;
}

QDataStream&
operator >> ( QDataStream& stream, ::heads::common::Message& message )
{
	stream >> message.type;
	stream >> message.id;
	stream >> message.content;

	return stream;
}
