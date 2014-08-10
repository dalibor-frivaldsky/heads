#include "QueryId.hpp"


#include <utility>




namespace heads {
namespace common
{

	QueryId::QueryId( QString id ):
	  id( std::move( id ) )
	{}

	QueryId::QueryId( const QueryId& other ):
	  id( other.id )
	{}

	QueryId::QueryId( QueryId&& other ):
	  id( std::move( other.id ) )
	{}

}}
