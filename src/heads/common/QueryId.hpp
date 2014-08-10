#pragma once


#include <QString>




namespace heads {
namespace common
{

	struct QueryId
	{
		QString		id;

		QueryId( QString id );
		QueryId( const QueryId& other );
		QueryId( QueryId&& other );
	};

}}
