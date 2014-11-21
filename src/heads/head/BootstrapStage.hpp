#pragma once


#include <rod/Find.hpp>
#include <rod/annotation/Requires.hpp>
#include <rod/match/Annotation.hpp>

#include <rod/factory/Create.hpp>

#include <heads/annotation/HeadDescriptor.hpp>
#include <heads/head/StageControl.hpp>




namespace heads {
namespace head {

	class BootstrapStage
	{
	public:
		using Requires = rod::annotation::Requires< StageControl< bool >& >;

		BootstrapStage( StageControl< bool >& )
		{}

		template< typename Context >
		void
		execute( Context& context )
		{
			using	HeadBootstrap = rod::Find<
										Context,
										rod::match::Annotation< annotation::IsHeadDescriptor >
									>::r::Head::r::Bootstrap;
			
			rod::factory::create< HeadBootstrap >( context )( context );
		}
	};
	
}}