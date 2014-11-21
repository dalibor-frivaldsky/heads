#pragma once


#include <heads/common/QueryId.hpp>




namespace heads {
namespace head {

	// An ugly hack to overcome rod/test/failing/msvc/multiSingleton bug
	struct HeadQueryId:
		public common::QueryId
	{
		inline
		HeadQueryId( QString id ):
		  QueryId( id )
		{}
	};
	
}}