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

		template< typename Class, typename Return, typename QueryReturn >
		struct GetQueryReturn< Return (Class::*)( QueryReturn ) const >
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
		query( Message&& message, Closure&& closure )
		{
			auto	cl = closure;

			QString queryId = requestPool.registerRequest(
				[=] ( const Message& response )
				{
					cl(
						response.readContent<
							typename detail::GetQueryReturn<
								decltype( &Closure::operator() ) >::r >() );

					queries.erase( response.id );
				});
			queries.insert( queryId );

			message.id = queryId;
			connection.sendMessage( message );
		}
	};
	
}}
