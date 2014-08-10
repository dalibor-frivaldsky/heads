#pragma once


#include <utility>

#include <QBuffer>
#include <QByteArray>
#include <QDataStream>




namespace heads {
namespace common
{

	class Response
	{
	private:
		QByteArray	rawResponse;
		
		
	public:
		Response( QByteArray rawResponse );


		template< typename ResponseContent >
		ResponseContent
		to()
		{
			ResponseContent		contentObject;
			QBuffer				contentBuffer( &rawResponse );
			QDataStream			contentStream( &contentBuffer );

			contentBuffer.open( QIODevice::ReadOnly );
			contentStream >> contentObject;
			contentBuffer.close();

			return std::move( contentObject );
		}
	};

}}
