#pragma once


#include <map>

#include <QString>

#include <heads/common/Message.hpp>
#include <heads/common/Request.hpp>




namespace heads {
namespace common
{

	class RequestPool
	{
	private:
		using requestMapType = std::map< QString, Request >;

		requestMapType	requestMap;


	public:
		void	add( Request request );
		void	processMessage( Message message );
	};

}}
