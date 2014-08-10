#include "RootConnection.hpp"


#include <utility>




namespace heads {
namespace head
{

	RootConnection::RootConnection()
	{}

	RootConnection::RootConnection( RootConnection&& other ):
	  common::Connection( std::move( other ) )
	{}

}}

