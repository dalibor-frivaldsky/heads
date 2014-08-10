#include "ServerNaming.hpp"




namespace heads {
namespace common
{

	QString
	ServerNaming::rootName()
	{
		return "memsource-te-root";
	}

	QString
	ServerNaming::headName( const HeadId& headId )
	{
		return "memsource-te-" + headId.id;
	}

}}
