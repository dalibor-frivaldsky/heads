#pragma once


#include <utility>

#include <rod/Extend.hpp>
#include <rod/Find.hpp>
#include <rod/Resolve.hpp>
#include <rod/Singleton.hpp>
#include <rod/match/Annotation.hpp>

#include <QMetaObject>
#include <QObject>

#include <heads/Bootstrapper.hpp>
#include <heads/Inform.hpp>
#include <heads/MessageControllers.hpp>
#include <heads/annotation/HeadDescriptor.hpp>
#include <heads/common/HeadId.hpp>
#include <heads/common/Message.hpp>
#include <heads/common/MessageDispatcher.hpp>
#include <heads/common/QueryIdProvider.hpp>
#include <heads/common/RequestPool.hpp>
#include <heads/head/BootstrapStage.hpp>
#include <heads/head/EstablishRootConnectionStage.hpp>
#include <heads/head/ExecuteStage.hpp>
#include <heads/head/HeadMessageDispatcher.hpp>
#include <heads/head/HeadServer.hpp>
#include <heads/head/RegisterStage.hpp>




namespace heads {
namespace head
{

	namespace detail
	{

		template< typename Context >
		struct DefineHeadContext
		{
		private:
			using HeadDescriptor = typename rod::Find<
										Context,
										rod::match::Annotation< ::heads::annotation::IsHeadDescriptor >
									>::r::Head::r;

			template< typename MessageControllers >
			struct Define;

			template< typename... MessageController >
			struct Define< MessageControllers< MessageController... > >
			{
				using r = typename rod::Extend< Context >
							::template With<
								MessageController...,
								rod::Singleton< HeadServer >,
								rod::Singleton< common::Connection > >::r;
			};

		public:
			using r = typename Define< typename HeadDescriptor::MessageControllers >::r;
		};
		
	}


	template< typename HeadsContext >
	class Head
	{
	private:
		typename detail::DefineHeadContext< HeadsContext >::r	headContext;

		common::Connection&		connection;
		HeadServer&				headServer;
		QApplication&			app;


	public:
		Head( HeadsContext& headsContext ):
		  headContext( headsContext ),
		  connection( rod::resolve< common::Connection& >( headContext ) ),
		  headServer( rod::resolve< HeadServer& >( headContext ) ),
		  app( rod::resolve< QApplication& >( headContext ) )
		{}

		void
		enter()
		{
			if( executeStage< EstablishRootConnectionStage >( headContext ) == false )
			{
				// TODO silently close the application?
				return;
			}
			executeStage< RegisterStage >( headContext );

			HeadMessageDispatcher< decltype( headContext ) >	headMessageDispatcher( headContext );
			headMessageDispatcher.start();

			if( executeStage< BootstrapStage, rod::Singleton< Bootstrapper > >( headContext ) )
			{
				runLogic();
			}
		}


	private:
		void
		runLogic()
		{
			using HeadDescriptor = 	rod::Find<
										HeadsContext,
										rod::match::Annotation< annotation::IsHeadDescriptor >
									>::r::Head::r;

			auto	queryService = common::QueryService< decltype( headContext ) >( headContext );
			auto	headContextWithQueryService = rod::extend( headContext )
														.with( queryService )();
			auto	headLogic = HeadDescriptor::template Logic<
									decltype( headContextWithQueryService ) >(
										headContextWithQueryService );

			headLogic.prepare();
			app.exec();
		}
	};

}}
