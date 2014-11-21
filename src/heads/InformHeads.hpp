#pragma once


#include <rod/Find.hpp>
#include <rod/match/Annotation.hpp>
#include <rod/match/Component.hpp>

#include <heads/Inform.hpp>
#include <heads/common/Message.hpp>
#include <heads/root/annotation/HeadIterator.hpp>




namespace heads
{

	namespace detail
	{

		struct InformHead
		{
		private:
			const common::Message&	message;


		public:
			InformHead( const common::Message& message ):
			  message( message )
			{}

			template< typename HeadContext >
			void
			operator () ( HeadContext& headContext ) const
			{
				inform( headContext, message );
			}
		};
		
	}

	template< typename Context >
	void
	informHeads( Context& context, const common::Message& message )
	{
		using	HeadIterator = 	typename rod::Find<
									Context,
									rod::match::Component<
										rod::match::Annotation< root::annotation::IsHeadIterator >
									>
								>::r::Head::r;

		rod::with( context,
		[&] ( common::HeadId& headId, HeadIterator& headIteartor )
		{
			headIteartor.each( detail::InformHead( message ), headId );
		});
	}
	
}