#pragma once


#include <rod/With.hpp>

#include <heads/common/Connection.hpp>
#include <heads/common/Message.hpp>
#include <heads/common/ProtocolWriter.hpp>
#include <heads/common/QueryId.hpp>




namespace heads {

	template< typename Context, typename ReplyContent >
	void
	reply( Context& context, ReplyContent&& replyContent )
	{
		rod::with( context,
		[&] ( common::Connection& connection, common::QueryId& queryId )
		{
			common::Message	message( "", queryId.id );
			message.writeContent( replyContent );

			connection.sendMessage( message );
		});
	}

}