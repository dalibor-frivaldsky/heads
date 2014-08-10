#pragma once


#include <list>

#include <QByteArray>
#include <QIODevice>

#include <heads/common/Message.hpp>




namespace heads {
namespace common
{

	class ProtocolReader
	{
	private:
		int						nextMessageSize = -1;
		int						remainingChunkSize = sizeof( int );
		QByteArray				accumulator;
		std::list< Message >	readMessages;


	public:
		ProtocolReader();
		ProtocolReader( const ProtocolReader& ) = delete;
		ProtocolReader( ProtocolReader&& other );
		~ProtocolReader();

		ProtocolReader&	operator = ( const ProtocolReader& ) = delete;
		ProtocolReader&	operator = ( ProtocolReader&& other );


		void	readFrom( QIODevice& inputDevice );
		bool	availableMessages() const;
		Message	get();
	};

}}
