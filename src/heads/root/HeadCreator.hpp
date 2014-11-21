#pragma once


#include <utility>

#include <heads/common/Socket.hpp>




namespace heads {
namespace root
{

	struct HeadCreator
	{
	public:
		virtual void	createFromReadSocket( common::Socket readSocket ) = 0;
	};
	
}}