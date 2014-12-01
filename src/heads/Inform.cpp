#include "Inform.hpp"




namespace heads
{

	Inform::Inform( common::Connection& connection ):
	  connection( connection )
	{}

	void
	Inform::operator () ( const common::Message& message )
	{
		connection.sendMessage( message );
	}
	
}