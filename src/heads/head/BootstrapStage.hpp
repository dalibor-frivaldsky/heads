#pragma once


#include <rod/annotation/Requires.hpp>
#include <rod/factory/Factory.hpp>

#include <heads/head/StageControl.hpp>




namespace heads {
namespace head {

	template< typename HeadBootstrap >
	class BootstrapStage
	{
	private:
		HeadBootstrap	headBootstrap;


	public:
		using Requires = rod::annotation::Requires<
							StageControl< bool >&,
							rod::factory::Factory< HeadBootstrap > >;

		BootstrapStage(
			StageControl< bool >&,
			rod::factory::Factory< HeadBootstrap > factory ):
		  headBootstrap( factory.create() )
		{}

		template< typename Context >
		void
		execute( Context& context )
		{
			headBootstrap( context );
		}
	};
	
}}