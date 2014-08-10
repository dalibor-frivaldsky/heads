#pragma once


#include <QEventLoop>




namespace heads
{

	class Bootstrapper
	{
	private:
		QEventLoop&		eventLoop;
		bool			proceedToLogic = false;


	public:
		Bootstrapper( QEventLoop& eventLoop );
		Bootstrapper( const Bootstrapper& ) = delete;
		Bootstrapper( Bootstrapper&& other ) = delete;


		void	proceed();
		void	quit();

		bool	shouldProceed() const;
	};

}