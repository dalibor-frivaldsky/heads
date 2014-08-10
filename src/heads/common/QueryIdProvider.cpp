#include "QueryIdProvider.hpp"




namespace heads {
namespace common
{

	QueryId
	QueryIdProvider::nextId()
	{
		int	n = next;
		++next;
		return QueryId { QString::number( n ) };
	}

}}
