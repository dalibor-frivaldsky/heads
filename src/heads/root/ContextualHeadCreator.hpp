#pragma once


#include <utility>

#include <heads/common/Socket.hpp>
#include <heads/root/HeadCreator.hpp>




namespace heads {
namespace root
{

	template< typename HeadListeners >
	struct ContextualHeadCreator:
		public HeadCreator
	{
	private:
		HeadListeners&	headListeners;


	public:
		ContextualHeadCreator( HeadListeners& headListeners ):
		  headListeners( headListeners )
		{}

		virtual
		void
		createFromReadSocket( common::Socket readSocket )
		{
			headListeners.incomingHeadConnection( std::move( readSocket ) );
		}
	};
	
}}