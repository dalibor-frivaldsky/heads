#pragma once


#include <type_traits>

#include <rod/Extend.hpp>
#include <rod/Resolve.hpp>
#include <rod/Singleton.hpp>
#include <rod/annotation/Requires.hpp>
#include <rod/debug/Inspect.hpp>

#include <rod/factory/Factory.hpp>

#include <QApplication>

#include <heads/common/QueryService.hpp>
#include <heads/head/StageControl.hpp>




namespace heads {
namespace head
{

	namespace detail
	{

		template< typename Stage >
		struct GetStageResult
		{
		private:
			using Requirements = typename rod::annotation::GetRequirements< Stage >::r;


			template< typename T >
			struct SelectStageControl;

			template< typename Result >
			struct SelectStageControl< StageControl< Result >& >
			{
				enum { r = true };
			};

			template< typename T >
			struct SelectStageControl
			{
				enum { r = false };
			};


			template< typename StageControl >
			struct GetResult
			{
				using r = typename std::decay< StageControl >::type::Result;
			};


		public:
			using r = typename GetResult<
						typename Requirements::template Select< SelectStageControl >::r::Head::r >::r;
		};


		template< typename Stage, typename... AdditionalContext >
		struct CreateStageContext
		{
		private:
			using Control = StageControl< typename GetStageResult< Stage >::r >;


		public:
			template< typename Context >
			static
			typename rod::Extend< Context >
				::template With< rod::Singleton< Control >, AdditionalContext... >::r
			create( Context& context )
			{
				return typename rod::Extend< Context >
							::template With< rod::Singleton< Control >, AdditionalContext... >::r( context );
			}
		};
		
	}


	template< typename Stage, typename... AdditionalContext, typename Context >
	typename detail::GetStageResult< Stage >::r
	executeStage( Context& context )
	{
		using	StageResult = typename detail::GetStageResult< Stage >::r;
		auto	stageContext = detail::CreateStageContext< Stage, AdditionalContext... >::create( context );
		auto	stageFactory = rod::resolve< rod::factory::Factory< Stage > >( stageContext );
		auto	stage = stageFactory.create();

		auto	queryService = common::QueryService< decltype( stageContext ) >( stageContext );
		auto	stageContextWithQueryService = rod::extend( stageContext )
													.with( queryService )();

		stage.execute( stageContextWithQueryService );
		rod::resolve< QApplication& >( stageContext ).exec();

		return rod::resolve< StageControl< StageResult >& >( stageContext ).getResult();
	}
	
}}