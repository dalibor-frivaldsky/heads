#pragma once


#include <rod/Dispatcher.hpp>

#include <QString>

#include <heads/annotation/MessageController.hpp>
#include <heads/common/Message.hpp>




namespace heads {
namespace common
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
		template< typename Controller, typename ContentType >
		struct CallController;

		template< typename Controller >
		struct CallController< Controller, void >
		{
			static
			void
			call( Controller& controller, Message& )
			{
				controller();
			}
		};

		template< typename Controller, typename ContentType >
		struct CallController
		{
			static
			void
			call( Controller& controller, Message& message )
			{
				controller( message.readContent< ContentType >() );
			}
		};


		template< template< typename > class MessageHandler, typename Context, typename... ToInject >
		static
		void
		perform( Context* context, Message&& message, ToInject&&... toInject )
		{
			auto	handler = rod::create< MessageHandler >( context, std::forward< ToInject >( toInject )... );
			using	contentType = typename decltype( handler )::MessageController::Content;

			CallController< decltype( handler ), contentType >::call( handler, message );
		}
	};

	template< typename Context >
	auto
	messageDispatcher( Context* context )
		-> decltype( rod::dispatcher<
						annotation::IsMessageController,
						MessageHandle,
						MessagePerformer >( context ) )
	{
		return rod::dispatcher<
					annotation::IsMessageController,
					MessageHandle,
					MessagePerformer >( context );
	}

}}
