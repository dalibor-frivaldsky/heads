#pragma once


#include <heads/common/Connection.hpp>




namespace heads {
namespace head
{

	class RootConnection:
		public common::Connection
	{
	public:
		RootConnection();
		RootConnection( RootConnection&& other );
	};

}}
