#pragma once


#include <rod/With.hpp>

#include <heads/Topic.hpp>
#include <heads/common/Connection.hpp>
#include <heads/common/Message.hpp>




namespace heads
{

	template< typename Context >
	void
	inform( Context& context, const common::Message& message )
	{
		rod::with( context,
		[&] ( common::Connection& connection )
		{
			connection.sendMessage( message );
		});
	}

}