#pragma once


#include <rod/With.hpp>

#include <QMetaObject>
#include <QObject>

#include <heads/root/HeadCreator.hpp>
#include <heads/root/RootServer.hpp>




namespace heads {
namespace root
{

	template< typename RootContext >
	class HeadConnectionListener
	{
	private:
		RootContext&	rootContext;

		QMetaObject::Connection	onHeadConnectedConnection;


	public:
		HeadConnectionListener( RootContext& rootContext ):
		  rootContext( rootContext )
		{}

		~HeadConnectionListener()
		{
			QObject::disconnect( onHeadConnectedConnection );
		}

		void
		listen()
		{
			rod::with( rootContext,
			[&,this] ( RootServer& rootServer )
			{
				onHeadConnectedConnection = rootServer.onHeadConnected(
				[=] ( common::Socket headReadSocket )
				{
					rod::with( rootContext,
					[&] ( HeadCreator& headCreator )
					{
						headCreator.createFromReadSocket( std::move( headReadSocket ) );
					});
				});
			});
		}
	};
	
}}