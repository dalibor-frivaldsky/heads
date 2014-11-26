#pragma once


#include <functional>
#include <set>

#include <rod/Resolve.hpp>

#include <heads/common/Connection.hpp>
#include <heads/common/Message.hpp>
#include <heads/common/RequestPool.hpp>
#include <heads/common/annotation/QueryProvider.hpp>




namespace heads {
namespace common
{

	namespace detail
	{

		// TODO move to rod
		template< typename CallbackOp >
		struct GetQueryReturn;

		template< typename Class, typename Return, typename Context, typename QueryReturn >
		struct GetQueryReturn< Return (Class::*)( Context, QueryReturn ) const >
		{
			using r = QueryReturn;
		};
		
	}


	template< typename Context >
	class QueryService
	{
	private:
		Context&			context;

		RequestPool&		requestPool;
		Connection&			connection;

		std::set< QString >	queries;


	public:
		using QueryProvider = annotation::QueryProvider;

		QueryService( Context& context ):
		  context( context ),
		  requestPool( rod::resolve< RequestPool& >( context ) ),
		  connection( rod::resolve< Connection& >( context ) )
		{}

		~QueryService()
		{
			for( auto& queryId: queries )
			{
				requestPool.unregisterRequest( queryId );
			}
		}

		template< typename Closure >
		void
		query( Message& message, Closure&& closure )
		{
			QString queryId = requestPool.registerRequest(
				[=] ( const Message& response )
				{
					auto	closureOp = &Closure::template operator()< Context >;
					closure(
						context,
						response.readContent<
							typename detail::GetQueryReturn< decltype( closureOp ) >::r >() );

					queries.erase( response.id );
				});
			queries.insert( queryId );

			message.id = queryId;
			connection.sendMessage( message );
		}
	};
	
}}
