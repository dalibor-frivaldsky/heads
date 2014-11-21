#pragma once


#include <rod/Find.hpp>
#include <rod/With.hpp>
#include <rod/match/Component.hpp>
#include <rod/match/Annotation.hpp>

#include <heads/common/Connection.hpp>
#include <heads/common/HeadId.hpp>
#include <heads/common/QueryId.hpp>
#include <heads/root/annotation/ReduceService.hpp>




namespace heads
{

	template< typename ReduceType, typename MapOp, typename ReduceOp, typename Context >
	void
	reduce( Context& context )
	{
		rod::with( context,
		[&] ( common::HeadId& headId, common::QueryId& queryId )
		{
			using	ReduceService =	typename rod::Find<
										Context,
										rod::match::Component<
											rod::match::Annotation< root::annotation::IsReduceService >
										>
									>::r::Head::r;
			auto&	reduceService = rod::resolve< ReduceService& >( context );

			reduceService.template reduce< ReduceType, MapOp, ReduceOp >( headId, queryId );
		});
	}

}