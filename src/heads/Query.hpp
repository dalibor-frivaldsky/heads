#pragma once


#include <functional>

#include <rod/With.hpp>
#include <rod/factory/Factory.hpp>

#include <QString>

#include <heads/Topic.hpp>
#include <heads/common/Connection.hpp>
#include <heads/common/Message.hpp>
#include <heads/common/Request.hpp>
#include <heads/common/RequestPool.hpp>
#include <heads/common/Response.hpp>




namespace heads
{

	namespace queryDetail
	{

		// TODO move to rod
		template< typename LambdaOp >
		struct GetQueryReturn;

		template< typename Class, typename Return, typename QueryReturn >
		struct GetQueryReturn< Return (Class::*)( QueryReturn ) const >
		{
			using r = QueryReturn;
		};

	}


	template< typename Context, typename Lambda >
	void
	query( Context* context, common::Message message, Lambda&& lambda )
	{
		rod::with( context,
		[=] ( rod::factory::Factory< common::Request > requestFactory, common::RequestPool& requestPool )
		{
			common::Request	r = requestFactory.create();

			r.setMessage( std::move( message ) );
			r.withResponse(
			[=] ( common::Response response )
			{
				lambda( response.to< typename queryDetail::GetQueryReturn< decltype( &Lambda::operator () ) >::r >() );
			});
			requestPool.add( std::move( r ) );
		});
	}

}