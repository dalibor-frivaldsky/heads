#pragma once


#include <rod/annotation/Requires.hpp>

#include <heads/head/StageControl.hpp>




namespace heads
{

	class Bootstrapper
	{
	private:
		head::StageControl< bool >&	stageControl;


	public:
		using Requires = rod::annotation::Requires< head::StageControl< bool >& >;

		Bootstrapper( head::StageControl< bool >& stageControl );


		void	proceed();
		void	quit();
	};

}