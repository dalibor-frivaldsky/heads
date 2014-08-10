#include "HeadId.hpp"


#include <utility>




namespace heads {
namespace common
{

	HeadId::HeadId()
	{}

	HeadId::HeadId( QString id ):
	  id( std::move( id ) )
	{}

	HeadId::HeadId( const HeadId& other ):
	  id( other.id )
	{}

	HeadId::HeadId( HeadId&& other ):
	  id( std::move( other.id ) )
	{}


	bool
	HeadId::operator < ( const HeadId& right ) const
	{
		return this->id < right.id;
	}

}}


QDataStream&
operator << ( QDataStream& stream, const ::heads::common::HeadId& headId )
{
	stream << headId.id;

	return stream;
}

QDataStream&
operator >> ( QDataStream& stream, ::heads::common::HeadId& headId )
{
	stream >> headId.id;

	return stream;
}
