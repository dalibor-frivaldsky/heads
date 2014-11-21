#pragma once


#include <functional>
#include <map>

#include <rod/annotation/Requires.hpp>

#include <QString>

#include <heads/common/Message.hpp>
#include <heads/common/QueryIdProvider.hpp>




namespace heads {
namespace common
{

	class RequestPool
	{
	private:
		using	RequestCallback = std::function< void( const Message& ) >;

		QueryIdProvider&	queryIdProvider;

		std::map< QString, RequestCallback >	requestMap;


	public:
		using Requires = rod::annotation::Requires< QueryIdProvider& >;

		RequestPool( QueryIdProvider& queryIdProvider );

		QString	registerRequest( RequestCallback requestCallback );
		void	unregisterRequest( const QString& requestId );
		void	processMessage( const Message& message );
	};

}}
