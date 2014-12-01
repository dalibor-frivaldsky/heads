#pragma once


#include <utility>

#include <heads/common/Message.hpp>




namespace heads
{

	template< typename Content >
	common::Message
	topic( QString name, const Content& content )
	{
		common::Message	m( name, "" );
		m.writeContent( content );

		return std::move( m );
	}

	common::Message	topic( QString name );

}