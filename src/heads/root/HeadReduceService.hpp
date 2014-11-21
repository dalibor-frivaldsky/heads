#pragma once


#include <utility>

#include <rod/Find.hpp>
#include <rod/Resolve.hpp>
#include <rod/Within.hpp>
#include <rod/match/Component.hpp>
#include <rod/match/Annotation.hpp>

#include <heads/common/Connection.hpp>
#include <heads/common/HeadId.hpp>
#include <heads/common/QueryId.hpp>
#include <heads/root/annotation/HeadIterator.hpp>
#include <heads/root/annotation/ReduceService.hpp>
#include <heads/root/annotation/ReductionPool.hpp>




namespace heads {
namespace root
{

	namespace detail
	{

		template< typename ReduceType, typename ReduceOp >
		struct PerformReduce
		{
		private:
			ReduceType	reduceType;


		public:
			PerformReduce( ReduceType reduceType ):
			  reduceType( std::move( reduceType ) )
			{}

			template< typename Context >
			void
			operator () ( Context& context )
			{
				ReduceOp()( context, std::move( reduceType ) );
			}
		};

		template< typename MapProviderOp, typename MapOp >
		struct PerformMap
		{
		private:
			MapProviderOp	mapProviderOp;


		public:
			PerformMap( MapProviderOp mapProviderOp ):
			  mapProviderOp( std::move( mapProviderOp ) )
			{}

			template< typename Context >
			void
			operator () ( Context& context )
			{
				MapOp()( context, std::move( mapProviderOp ) );
			}
		};
		
	}


	template< typename Context >
	class HeadReduceService
	{
	private:
		Context&	context;


	public:
		using ReduceService = annotation::ReduceService;


		HeadReduceService( Context& context ):
		  context( context )
		{}

		template< typename ReduceType, typename MapOp, typename ReduceOp >
		void
		reduce( common::HeadId& callerId, common::QueryId queryId )
		{
			using	HeadIterator =	typename rod::Find<
										Context,
										rod::match::Component<
											rod::match::Annotation< root::annotation::IsHeadIterator >
										>
									>::r::Head::r;
			auto&	headIterator = rod::resolve< HeadIterator& >( context );

			using	ReductionPool =	typename rod::Find<
										Context,
										rod::match::Component<
											rod::match::Annotation< root::annotation::IsReductionPool >
										>
									>::r::Head::r;
			auto&	reductionPool = rod::resolve< ReductionPool& >( context );


			// TODO unsafe use of this, get reduction id and unregister all reductions of this service
			auto	reduceOp = 
			[=] ( ReduceType result )
			{
				common::QueryId	qid( queryId );
				rod::within(
					context,
					qid,
				detail::PerformReduce< ReduceType, ReduceOp >( std::move( result ) ) );
			};
			auto	mapProviderOp = reductionPool.prepareReduction< ReduceType >(
										headIterator.getNumberOfListening( callerId ),
										reduceOp );

			headIterator.each( 
				detail::PerformMap<
					decltype( mapProviderOp ),
					MapOp
				>( mapProviderOp ),
				callerId );

			reductionPool.check();
		}
	};

}}
