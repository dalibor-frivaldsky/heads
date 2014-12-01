#pragma once


#include <set>
#include <utility>

#include <rod/Find.hpp>
#include <rod/Resolve.hpp>
#include <rod/annotation/Requires.hpp>
#include <rod/match/Annotation.hpp>
#include <rod/match/Component.hpp>

#include <rod/factory/Factory.hpp>

#include <QString>

#include <heads/Topic.hpp>
#include <heads/common/Connection.hpp>
#include <heads/common/Message.hpp>
#include <heads/common/RequestPool.hpp>
#include <heads/common/annotation/QueryProvider.hpp>




namespace heads
{

	template< typename Context, typename Closure >
	void
	query( Context& context, common::Message message, Closure&& closure )
	{
		using QueryProvider =	typename rod::Find<
									Context,
									rod::match::Component<
										rod::match::Annotation< common::annotation::IsQueryProvider >
									>
								>::r::Head::r;
		auto& queryProvider = rod::resolve< QueryProvider& >( context );

		queryProvider.query( message, std::move( closure ) );
	}


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


	class Query
	{
	private:
		common::Connection&			connection;
		common::RequestPool&		requestPool;

		std::set< QString >	queries;


	public:
		using Requires = rod::annotation::Requires<
							common::Connection&,
							common::RequestPool& >;

		Query( common::Connection& connection, common::RequestPool& requestPool );
		Query( const Query& other );
		Query( Query&& other );
		~Query();

		template< typename Closure >
		void
		operator () ( common::Message& message, Closure&& closure )
		{
			QString queryId = requestPool.registerRequest(
			[=] ( const common::Message& response )
			{
				closure(
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

}