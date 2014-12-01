#pragma once


#include <rod/annotation/Resolver.hpp>

#include <rod/factory/Create.hpp>

#include <heads/Inform.hpp>
#include <heads/Query.hpp>




namespace heads
{

	namespace detail
	{

		template< typename HeadsType >
		struct HeadsSelector;

		template<>
		struct HeadsSelector< Query >
		{
			enum { r = true };
		};

		template<>
		struct HeadsSelector< Inform >
		{
			enum { r = true };
		};

		template< typename OtherType >
		struct HeadsSelector
		{
			enum { r = false };
		};
		
	}


	struct HeadsResolver
	{
		using Resolver = rod::annotation::Resolver< detail::HeadsSelector >;

		template< typename HeadsType, typename Context >
		static
		HeadsType
		resolve( Context& context )
		{
			return rod::factory::create< HeadsType >( context );
		}
	};
	
}