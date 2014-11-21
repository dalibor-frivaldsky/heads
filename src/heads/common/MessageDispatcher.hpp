#pragma once


#include <utility>

#include <rod/Dispatcher.hpp>
#include <rod/Find.hpp>
#include <rod/Resolve.hpp>
#include <rod/Within.hpp>
#include <rod/match/Annotation.hpp>

#include <rod/factory/Create.hpp>

#include <QString>

#include <heads/annotation/MessageController.hpp>
#include <heads/common/Message.hpp>




namespace heads {
namespace common
{

	namespace detail
	{

		struct MessageHandle
		{
			template< typename MessageHandler >
			static
			QString
			handle()
			{
				return MessageHandler::endpoint();
			}
		};

		struct MessagePerformer
		{
			template< typename MessageHandler, typename ContentType >
			struct CallMessageHandler;

			template< typename MessageHandler >
			struct CallMessageHandler< MessageHandler, void >
			{
				template< typename Context >
				static
				void
				call( MessageHandler& handler, Context& context, const Message& )
				{
					handler( context );
				}
			};

			template< typename MessageHandler, typename ContentType >
			struct CallMessageHandler
			{
				template< typename Context >
				static
				void
				call( MessageHandler& handler, Context& context, const Message& message )
				{
					handler( context, message.readContent< ContentType >() );
				}
			};


			template< typename MessageHandler >
			struct Perform
			{
			private:
				const Message&	message;


			public:
				Perform( const Message& message ):
				  message( message )
				{}

				template< typename Context >
				void
				operator () ( Context& context )
				{
					auto	handler = rod::factory::create< MessageHandler >( context );
					using	ContentType = typename MessageHandler::MessageController::Content;

					CallMessageHandler< MessageHandler, ContentType >::call( handler, context, message );
				}
			};


			template< typename MessageHandler, typename Context, typename... ToInject >
			static
			void
			perform( Context& context, const Message& message, ToInject&&... toInject )
			{
				rod::within(
					context,
					std::forward< ToInject >( toInject )...,
				Perform< MessageHandler >( message ) );
			}
		};
		
	}


	template< typename Context >
	struct CreateMessageDispatcher
	{
		using r = rod::Dispatcher<
					typename rod::Find<
						Context,
						rod::match::Annotation< heads::annotation::IsMessageController > >::r,
					detail::MessageHandle,
					detail::MessagePerformer >;
	};

}}
