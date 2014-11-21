#pragma once


#include <utility>

#include <rod/Find.hpp>
#include <rod/Resolve.hpp>
#include <rod/match/Annotation.hpp>
#include <rod/match/Component.hpp>

#include <rod/factory/Factory.hpp>

#include <QString>

#include <heads/Topic.hpp>
#include <heads/common/Message.hpp>
#include <heads/common/annotation/QueryProvider.hpp>




namespace heads
{

	template< typename Context, typename Closure >
	void
	query( Context& context, common::Message message, Closure&& closure )
	{
		using QueryProvider =	typename rod::Find<
									Context,
									rod::match::Component<
										rod::match::Annotation< common::annotation::IsQueryProvider >
									>
								>::r::Head::r;
		auto& queryProvider = rod::resolve< QueryProvider& >( context );

		queryProvider.query( message, std::move( closure ) );
	}

}