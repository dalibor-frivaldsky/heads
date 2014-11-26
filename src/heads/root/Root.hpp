#pragma once


#include <map>
#include <utility>

#include <rod/Resolve.hpp>
#include <rod/Singleton.hpp>
#include <rod/Within.hpp>

#include <heads/root/HeadConnectionListener.hpp>
#include <heads/root/ContextualHeadCreator.hpp>
#include <heads/root/HeadListeners.hpp>
#include <heads/root/RootServer.hpp>




namespace heads {
namespace root
{

	namespace detail
	{
        
        struct InitializeServer
        {
            template< typename Context >
            void
            operator () ( Context& context )
            {
                HeadConnectionListener< Context >	headConnectionListener( context );
                headConnectionListener.listen();
                
                rod::resolve< RootServer& >( context ).listen();
                rod::resolve< QApplication& >( context ).exec();
            }
        };
    

		struct EnterRoot
		{
			template< typename RootContext >
			void
			operator () ( RootContext& rootContext )
			{
				HeadListeners< RootContext >	headListeners( rootContext );

				rod::within(
					rootContext,
					ContextualHeadCreator< decltype( headListeners ) >( headListeners ),
				InitializeServer() );
			}
		};
	
	}


	class Root
	{
	public:
		template< typename Context >
		void
		enter( Context& context )
		{
			rod::within<
				rod::Singleton< RootServer > >(
				context,
			detail::EnterRoot() );
		}
	};

}}
