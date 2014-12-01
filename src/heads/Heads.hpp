#pragma once


#include <rod/Singleton.hpp>
#include <rod/Within.hpp>
#include <rod/evaluable/EvaluableResolver.hpp>

#include <rod/factory/FactoryResolver.hpp>

#include <QCoreApplication>
#include <QMetaType>

#include <heads/HeadsResolver.hpp>
#include <heads/common/Options.hpp>
#include <heads/common/QueryIdProvider.hpp>
#include <heads/common/RequestPool.hpp>
#include <heads/common/ServerNaming.hpp>
#include <heads/head/Head.hpp>
#include <heads/root/Root.hpp>




namespace heads
{

	namespace detail
	{

		struct EnterHeads
		{
			template< typename HeadsContext >
			void
			operator () ( HeadsContext& headsContext )
			{
				if( QCoreApplication::arguments().contains( common::Options::startAsRoot() ) )
				{
					root::Root().enter( headsContext );
				}
				else
				{
					head::Head< HeadsContext >( headsContext ).enter();
				}
			}
		};
		
	}


	template< typename Context >
	void
	enter( Context& context )
	{
		qRegisterMetaType< QLocalSocket::LocalSocketError >( "LocalSocketError" );

		rod::within<
			rod::factory::FactoryResolver,
			rod::evaluable::EvaluableResolver,
			HeadsResolver,
			rod::Singleton< common::ServerNaming >,
			rod::Singleton< common::QueryIdProvider >,	// TODO MSVC2013 issue: QueryIdProvicer
			rod::Singleton< common::RequestPool > >(	// and RequestPool cannot be in 2 different context hierarchies
			context,
		detail::EnterHeads() );
	}

}