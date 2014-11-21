#pragma once


#include <utility>

#include <rod/annotation/Requires.hpp>

#include <QApplication>




namespace heads {
namespace head
{

	template< typename Result >
	class StageControl;

	template<>
	class StageControl< void >
	{
	public:
		using Result = void;


	private:
		QApplication&	app;


	public:
		using Requires = rod::annotation::Requires< QApplication& >;

		StageControl( QApplication& app ):
		  app( app )
		{}

		void
		done()
		{
			app.exit();
		}

		void
		getResult()
		{}
	};

	template< typename Result_ >
	class StageControl
	{
	public:
		using Result = Result_;


	private:
		QApplication&	app;

		Result	result;


	public:
		using Requires = rod::annotation::Requires< QApplication& >;

		StageControl( QApplication& app ):
		  app( app )
		{}

		void
		done( Result&& r )
		{
			result = std::forward< Result >( r );
			app.quit();
		}

		Result
		getResult()
		{
			return std::move( result );
		}
	};
	
}}