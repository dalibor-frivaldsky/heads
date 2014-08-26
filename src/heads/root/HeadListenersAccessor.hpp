#pragma once


#include <utility>

#include <heads/root/annotation/ListenersAccessor.hpp>




namespace heads {
namespace root
{

	template< typename Parent >
	class HeadListenersAccessor
	{
	private:
		Parent*		parent;


	public:
		using ListenersAccessor = annotation::ListenersAccessor;

		HeadListenersAccessor( Parent* parent ):
		  parent( parent )
		{}

		HeadListenersAccessor( HeadListenersAccessor&& other ):
		  parent( other.parent )
		{}


		template< typename EachOp, typename... ToForward >
		void
		eachExcept( const common::HeadId& except, ToForward&&... toForward )
		{
			using			Listeners = typename Parent::Listeners;
			Listeners&		listeners = parent->getListeners();
			
			for( auto& listenerPair: listeners.getContainer() )
			{
				auto&			listener = listenerPair.second;
				common::HeadId&	headId = listener.getId();

				if( except.id != headId.id &&
					listener.getStage() == ListenerStage::Listening )
				{
					EachOp()( listener, std::forward< ToForward >( toForward )... );
				}
			}
		}
	};
	
}}