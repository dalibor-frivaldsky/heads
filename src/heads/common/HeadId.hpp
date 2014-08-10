#pragma once


#include <QDataStream>
#include <QString>




namespace heads {
namespace common
{

	struct HeadId
	{
		QString		id;


		HeadId();
		HeadId( QString id );
		HeadId( const HeadId& other );
		HeadId( HeadId&& other );


		bool	operator < ( const HeadId& right ) const;
	};

}}


QDataStream&	operator << ( QDataStream& stream, const ::heads::common::HeadId& headId );
QDataStream&	operator >> ( QDataStream& stream, ::heads::common::HeadId& headId );
