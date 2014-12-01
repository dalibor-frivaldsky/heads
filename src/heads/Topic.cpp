#include "Topic.hpp"




namespace heads
{

	common::Message
	topic( QString name )
	{
		common::Message m( name, "" );

		return std::move( m );
	}
	
}