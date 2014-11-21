#pragma once


#include <QObject>

#include <heads/common/Message.hpp>
#include <heads/common/ProtocolReader.hpp>
#include <heads/common/Socket.hpp>




namespace heads {
namespace common
{

	class Connection:
		public QObject
	{
		Q_OBJECT


	private:
		Socket			readSocket;
		Socket			writeSocket;
		ProtocolReader	protocolReader;


	public:
		Connection();
		Connection( Socket readSocket, Socket writeSocket, ProtocolReader protocolReader );
		Connection( Connection&& other );
		~Connection();

		Connection( const Connection& ) = delete;

		const Socket&	getReadSocket() const;
		void			setReadSocket( Socket&& readSocket );
		const Socket&	getWriteSocket() const;
		void			setWriteSocket( Socket&& writeSocket );

		void	sendMessage( const Message& message );


	signals:
		void	messageReceived( const Message& message );


	private slots:
		void	onReadyRead();
	};

}}
