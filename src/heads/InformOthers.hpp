#pragma once


#include <heads/Inform.hpp>
#include <heads/common/Message.hpp>
#include <heads/root/annotation/ListenersAccessor.hpp>




namespace heads
{

	namespace informOthersDetail
	{

		struct ListenerOp
		{
			template< typename Listener >
			void
			operator () ( Listener& listener, const common::Message& message )
			{
				inform( &listener, message );
			}
		};
		
	}

	template< typename Context >
	void
	informOthers( Context* context, const common::Message& message )
	{
		rod::with( context,
		[=] ( common::HeadId& headId )
		{
			using	ListenersAccessor = typename Context::template FindRegisteredType< root::annotation::IsListenersAccessor >::r::Head::r;
			auto&	listenersAccessor = rod::resolve< ListenersAccessor& >( context );

			listenersAccessor.eachExcept< informOthersDetail::ListenerOp >( headId, message );
		});
	}
	
}