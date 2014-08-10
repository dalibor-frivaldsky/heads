#pragma once


#include <rod/Annotation.hpp>




namespace heads {
namespace annotation
{

	template< typename Content_ >
	struct MessageController
	{
		using Content = Content_;
	};

	ROD_Annotation( MessageController )

}}