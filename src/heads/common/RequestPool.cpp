#include "RequestPool.hpp"


#include <utility>




namespace heads {
namespace common
{

	void
	RequestPool::add( Request request )
	{
		requestMap.insert( std::make_pair( request.getId().id, std::move( request ) ) );
	}

	void
	RequestPool::processMessage( Message message )
	{
		requestMapType::iterator it = requestMap.find( message.id );

		if( it != requestMap.end() )
		{
			it->second.provideRawResponse( std::move( message.content ) );
		}
	}

}}
