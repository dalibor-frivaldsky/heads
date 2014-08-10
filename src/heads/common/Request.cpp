#include "Request.hpp"


#include <utility>

#include <heads/common/ProtocolWriter.hpp>




namespace heads {
namespace common
{

	Request::Request( QueryIdProvider& idProvider, Connection& connection ):
	  connection( connection ),
	  id( idProvider.nextId() )
	{}

	Request::Request( Request&& other ):
	  connection( other.connection ),
	  id( std::move( other.id ) ),
	  message( std::move( other.message ) ),
	  responseCallback( std::move( other.responseCallback ) )
	{}

	QueryId
	Request::getId() const
	{
		return id;
	}

	void
	Request::setMessage( Message message )
	{
		this->message = std::move( message );
		this->message.id = id.id;
	}

	void
	Request::withResponse( std::function< void( Response ) > responseCallback )
	{
		this->responseCallback = std::move( responseCallback );

		common::ProtocolWriter::write( *connection.getWriteSocket(), message );
	}

	void
	Request::provideRawResponse( QByteArray rawResponse )
	{
		responseCallback( Response( std::move( rawResponse ) ) );
	}

}}
