#include "Response.hpp"




namespace heads {
namespace common
{

	Response::Response( QByteArray rawResponse ):
	  rawResponse( std::move( rawResponse ) )
	{}

}}
