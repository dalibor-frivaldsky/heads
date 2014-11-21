#include "Bootstrapper.hpp"




namespace heads
{

	Bootstrapper::Bootstrapper( head::StageControl< bool >& stageControl ):
	  stageControl( stageControl )
	{}

	void
	Bootstrapper::proceed()
	{
		stageControl.done( true );
	}

	void
	Bootstrapper::quit()
	{
		stageControl.done( false );
	}

}