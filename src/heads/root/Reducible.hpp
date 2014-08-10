#pragma once


#include <heads/root/annotation/ReducibleDescriptor.hpp>




namespace heads {
namespace root {

	template< typename ReducibleType >
	struct Reducible
	{
		using ReducibleDescriptor = root::annotation::ReducibleDescriptor;

		using Type = ReducibleType;
	};

}}
