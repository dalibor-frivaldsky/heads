#pragma once


#include <heads/common/Connection.hpp>
#include <heads/common/HeadId.hpp>




namespace heads {
namespace root
{

	class ConnectionRegisterer
	{
	public:
		virtual void	registerConnection( common::HeadId headId, common::Connection connection ) = 0;
	};

}}
