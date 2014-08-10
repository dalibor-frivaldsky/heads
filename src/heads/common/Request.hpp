#pragma once


#include <functional>

#include <rod/annotation/ConstructWith.hpp>

#include <QByteArray>

#include <heads/common/Connection.hpp>
#include <heads/common/Message.hpp>
#include <heads/common/QueryIdProvider.hpp>
#include <heads/common/Response.hpp>




namespace heads {
namespace common
{

	class Request
	{
	private:
		Connection&		connection;

		QueryId		id;
		Message		message;

		std::function< void( Response ) >	responseCallback;

		
	public:
		using ConstructWith = rod::annotation::ConstructWith< QueryIdProvider&, Connection& >;
		Request( QueryIdProvider& idProvider, Connection& connection );

		Request( Request&& other );

		QueryId	getId() const;

		void	setMessage( Message message );
		void	withResponse( std::function< void( Response ) > responseCallback );
		void	provideRawResponse( QByteArray rawResponse );
	};

}}
