#pragma once


#include <rod/With.hpp>

#include <heads/Topic.hpp>
#include <heads/common/Connection.hpp>
#include <heads/common/Message.hpp>
#include <heads/common/ProtocolWriter.hpp>




namespace heads
{

	template< typename Context >
	void
	inform( Context* context, common::Message m )
	{
		rod::with( context,
		[&] ( common::Connection& connection )
		{
			common::ProtocolWriter::write( *connection.getWriteSocket(), m );
		});
	}

}