#pragma once


#include <rod/With.hpp>

#include <heads/common/Connection.hpp>
#include <heads/common/HeadId.hpp>
#include <heads/common/QueryId.hpp>
#include <heads/root/annotation/ReduceService.hpp>




namespace heads
{

	template< typename ReduceType, typename MapOp, typename ReduceOp, typename Context >
	void
	reduce( Context* context )
	{
		rod::with( context,
		[=] ( common::HeadId& headId, common::Connection& connection, common::QueryId& queryId )
		{
			using	ReduceService = typename Context::template FindRegisteredType< root::annotation::IsReduceService >::r::Head::r;
			auto&	reduceService = rod::resolve< ReduceService& >( context );

			reduceService.template reduce< ReduceType, MapOp, ReduceOp >( headId, connection, queryId );
		});
	}

}