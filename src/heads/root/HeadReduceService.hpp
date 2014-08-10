#pragma once


#include <utility>

#include <rod/Contextual.hpp>

#include <heads/common/Connection.hpp>
#include <heads/common/HeadId.hpp>
#include <heads/common/QueryId.hpp>
#include <heads/root/annotation/ReduceService.hpp>




namespace heads {
namespace root
{

	namespace headReduceServiceDetail
	{
	
		// TODO move to rod
		template< typename Context >
		class ContextCaller:
			public rod::Contextual< Context >
		{
		public:
			ROD_Contextual_Constructor( ContextCaller );

			ContextCaller( const ContextCaller< Context >& ) = delete;


			template< typename Op, typename... ToForward >
			void
			operator () ( ToForward&&... toForward )
			{
				Op()( this, std::forward< ToForward >( toForward )... );
			}
		};
	
	}


	template< typename Parent >
	class HeadReduceService
	{
	private:
		Parent*		parent;


	public:
		using ReduceService = annotation::ReduceService;


		HeadReduceService( Parent* parent ):
		  parent( parent )
		{}

		HeadReduceService( HeadReduceService&& other ):
		  parent( other.parent )
		{}


		template< typename ReduceType, typename MapOp, typename ReduceOp >
		void
		reduce( common::HeadId& callerId, common::Connection& callerConnection, common::QueryId queryId )
		{
			using			Listeners = typename Parent::Listeners;
			Listeners&		listeners = parent->getListeners();
			using			Reductions = typename Parent::Reductions;
			Reductions&		reductions = parent->getReductions();

			auto	reduceOp = 
			[this, &callerConnection, queryId] ( ReduceType result )
			{
				auto	cc = rod::create< headReduceServiceDetail::ContextCaller >( parent, callerConnection, common::QueryId( queryId ) );
				cc.operator () < ReduceOp > ( std::move( result ) );
			};
			auto	mapProviderOp = reductions.prepareReduction< ReduceType >( listeners.getNumberOfListening() - 1, reduceOp );

			for( auto& listener: listeners.getContainer() )
			{
				common::HeadId&	headId = rod::resolve< common::HeadId& >( &listener.second );

				if( callerId.id != headId.id &&
					listener.second.getStage() == ListenerStage::Listening )
				{
					common::Connection&	headConnection = rod::resolve< common::Connection& >( &listener.second );
					auto				caller = rod::create< headReduceServiceDetail::ContextCaller >( parent, headConnection );

					caller.operator () < MapOp >( mapProviderOp );
				}
			}

			checkReductions();
		}

		void
		checkReductions()
		{
			parent->getReductions().check();
		}
	};

}}
