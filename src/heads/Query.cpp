#include "Query.hpp"




namespace heads
{

	Query::Query( common::Connection& connection, common::RequestPool& requestPool ):
	  connection( connection ),
	  requestPool( requestPool )
	{}

	Query::Query( const Query& other ):
	  connection( other.connection ),
	  requestPool( other.requestPool )
	{}

	Query::Query( Query&& other ):
	  connection( other.connection ),
	  requestPool( other.requestPool )
	{}

	Query::~Query()
	{
		for( auto& queryId: queries )
		{
			requestPool.unregisterRequest( queryId );
		}
	}
	
}