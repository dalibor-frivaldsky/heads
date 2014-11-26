#pragma once


#include <heads/common/HeadId.hpp>
#include <heads/root/HeadListener.hpp>
#include <heads/root/annotation/HeadIterator.hpp>




namespace heads {
namespace root
{

	template< typename HeadListeners >
	class HeadIteratorService
	{
	private:
		HeadListeners&	headListeners;


	public:
		using HeadIterator = root::annotation::HeadIterator;

		HeadIteratorService( HeadListeners& headListeners ):
		  headListeners( headListeners )
		{}

		template< typename Closure >
		void
		each( Closure&& closure, common::HeadId except = common::HeadId() )
		{
			for( auto& headListener: headListeners.getListeners() )
			{
				auto&	listener = headListener.second;
				if( headListener.first != except.id
					&& 
					listener.getStage() == ListenerStage::Listening )
				{
					closure( listener.getContext() );
				}
			}
		}

		int
		getNumberOfListening( common::HeadId except = common::HeadId() )
		{
			int	num = 0;

			for( auto& headListener: headListeners.getListeners() )
			{
				auto&	listener = headListener.second;
				if( headListener.first != except.id
					&& 
					listener.getStage() == ListenerStage::Listening )
				{
					++num;
				}
			}

			return num;
		}
	};
	
}}