#include "Bootstrapper.hpp"




namespace heads
{

	Bootstrapper::Bootstrapper( QEventLoop& eventLoop ):
	  eventLoop( eventLoop )
	{}

	void
	Bootstrapper::proceed()
	{
		eventLoop.quit();
		proceedToLogic = true;
	}

	void
	Bootstrapper::quit()
	{
		eventLoop.quit();
	}

	bool
	Bootstrapper::shouldProceed() const
	{
		return proceedToLogic;
	}

}