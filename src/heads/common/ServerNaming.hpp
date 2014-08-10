#pragma once


#include <QString>

#include <heads/common/HeadId.hpp>




namespace heads {
namespace common
{

	class ServerNaming
	{
	public:
		QString	rootName();
		QString	headName( const HeadId& headId );
	};

}}
