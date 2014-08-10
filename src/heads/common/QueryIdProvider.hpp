#pragma once


#include <heads/common/QueryId.hpp>




namespace heads {
namespace common
{

	class QueryIdProvider
	{
	private:
		int		next = 0;

	public:
		QueryId		nextId();
	};

}}
