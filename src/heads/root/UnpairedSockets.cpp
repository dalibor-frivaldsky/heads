#include "UnpairedSockets.hpp"


#include <utility>




namespace heads {
namespace root{

	namespace unpairedSockets
	{
	
		UnpairedSocket::UnpairedSocket( common::Socket readSocket ):
			readSocket( std::move( readSocket ) )
		{}

		UnpairedSocket::UnpairedSocket( UnpairedSocket&& other ):
			readSocket( std::move( other.readSocket ) ),
			protocolReader( std::move( other.protocolReader ) ),
			connection( std::move( other.connection ) )
		{}
	
	}


	UnpairedSockets::iterator
	UnpairedSockets::addReadSocket( common::Socket readSocket )
	{
		return unpairedReadSockets.insert( unpairedReadSockets.begin(), unpairedSockets::UnpairedSocket( std::move( readSocket ) ) );
	}

	void
	UnpairedSockets::removeUnpairedReadSocket( iterator it )
	{
		unpairedReadSockets.erase( it );
	}

}}
