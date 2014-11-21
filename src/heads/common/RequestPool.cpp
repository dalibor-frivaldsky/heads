#include "RequestPool.hpp"


#include <utility>




namespace heads {
namespace common
{

	RequestPool::RequestPool( QueryIdProvider& queryIdProvider ):
	  queryIdProvider( queryIdProvider )
	{}

	QString
	RequestPool::registerRequest( RequestCallback requestCallback )
	{
		QString	requestId = queryIdProvider.nextId().id;

		requestMap[ requestId ] = std::move( requestCallback );

		return requestId;
	}

	void
	RequestPool::unregisterRequest( const QString& requestId )
	{
		requestMap.erase( requestId );
	}

	void
	RequestPool::processMessage( const Message& message )
	{
		auto	requestIt = requestMap.find( message.id );

		if( requestIt != requestMap.end() )
		{
			requestIt->second( message );
			requestMap.erase( requestIt );
		}
	}

}}
