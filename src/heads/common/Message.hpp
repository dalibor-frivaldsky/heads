#pragma once


#include <utility>

#include <QBuffer>
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QString>




namespace heads {
namespace common
{

	struct Message
	{
	public:
		QString		type;
		QString		id;
		QByteArray	content;


					Message();
					Message( Message&& other );
		Message&	operator = ( Message&& other );


		template< typename TypeArg, typename IdArg >
		Message( TypeArg&& type, IdArg&& id ):
		  type( std::forward< TypeArg >( type ) ),
		  id( std::forward< IdArg >( id ) )
		{}

		template< typename Content >
		Message&
		writeContent( const Content& contentObject )
		{
			QBuffer		contentBuffer( &content );
			QDataStream	contentStream( &contentBuffer );
			
			contentBuffer.open( QIODevice::WriteOnly );
			contentStream << contentObject;
			contentBuffer.close();

			return *this;
		}

		template< typename Content >
		Content
		readContent() const
		{
			Content		contentObject;
			QByteArray	contentCopy( content );
			QBuffer		contentBuffer( &contentCopy );
			QDataStream	contentStream( &contentBuffer );

			contentBuffer.open( QIODevice::ReadOnly );
			contentStream >> contentObject;
			contentBuffer.close();

			return std::move( contentObject );
		}
	};

}}


QDataStream&	operator << ( QDataStream& stream, const ::heads::common::Message& message );
QDataStream&	operator >> ( QDataStream& stream, ::heads::common::Message& message );
