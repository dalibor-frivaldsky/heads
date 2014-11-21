#pragma once


#include <heads/common/HeadId.hpp>




namespace heads {
namespace root
{

	class HeadRegisterer
	{
	public:
		virtual void	registerHeadWithId( common::HeadId headId ) = 0;
	};
	
}}