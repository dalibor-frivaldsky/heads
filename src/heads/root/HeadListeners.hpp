#pragma once


#include <map>
#include <utility>

#include <QString>

#include <heads/common/HeadId.hpp>




namespace heads {
namespace root
{

	template< typename HeadListener >
	class HeadListeners
	{
	private:
		using Listeners = std::map< QString, HeadListener >;

		Listeners	listeners;


	public:
		void
		add( const common::HeadId& headId, HeadListener listener )
		{
			listeners.insert( std::make_pair( headId.id , std::move( listener ) ) );
		}

		bool
		has( const common::HeadId& headId ) const
		{
			return listeners.find( headId.id ) != listeners.end();
		}

		HeadListener&
		get( const common::HeadId& headId )
		{
			return listeners.find( headId.id )->second;
		}

		int
		getNumberOfListening() const
		{
			int	listening = 0;

			for( auto& listener: listeners )
			{
				if( listener.second.getStage() == ListenerStage::Listening )
				{
					++listening;
				}
			}

			return listening;
		}

		// TODO provide iteration capability instead
		Listeners&
		getContainer()
		{
			return listeners;
		}
	};

}}
