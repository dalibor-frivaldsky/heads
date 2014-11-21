#pragma once


#include <utility>

#include <rod/Resolve.hpp>

#include <heads/annotation/MessageController.hpp>
#include <heads/common/HeadId.hpp>
#include <heads/root/HeadRegisterer.hpp>




namespace heads {
namespace root
{

	class RegisterMessage
	{
	private:


	public:
		using MessageController = ::heads::annotation::MessageController< common::HeadId >;

		static
		QString
		endpoint()
		{
			return "head/register";
		}

		template< typename Context >
		void
		operator () ( Context& context, common::HeadId headId )
		{
			rod::resolve< HeadRegisterer& >( context ).registerHeadWithId( std::move( headId ) );
		}

	};

}}
