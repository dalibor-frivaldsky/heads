#pragma once


#include <functional>
#include <list>
#include <map>
#include <utility>

#include <rod/Generate.hpp>




namespace heads {
namespace root
{

	namespace reductionsDetail
	{

		template< typename ReducibleType >
		class Reduction
		{
		private:
			int										mapCount;
			ReducibleType							result;
			std::function< void( ReducibleType ) >	reductionOp;


		public:
			Reduction( int mapCount, std::function< void( ReducibleType ) > reductionOp ):
			  mapCount( mapCount ),
			  reductionOp( reductionOp )
			{}

			
			ReducibleType&
			getMapTarget()
			{
				--mapCount;
				return result;
			}

			int
			getCount() const
			{
				return mapCount;
			}

			void
			performReduction()
			{
				reductionOp( std::move( result ) );
			}
		};
	

		template< typename ReducibleType >
		class SpecificReductions
		{
		private:
			using Reductions = std::list< Reduction< ReducibleType > >;

			Reductions			reductions;


		public:
			std::function< ReducibleType&() >
			prepareReduction( int mapCount, std::function< void( ReducibleType ) > reductionOp )
			{
				Reductions::iterator		reductionIt = reductions.insert( reductions.begin(), Reduction< ReducibleType >( mapCount, reductionOp ) );

				return
				[=] () -> ReducibleType&
				{
					return reductionIt->getMapTarget();
				};
			}

			void
			check()
			{
				for( auto& reduction: reductions )
				{
					if( reduction.getCount() <= 0 )
					{
						reduction.performReduction();
					}
				}

				reductions.remove_if(
				[] ( Reduction< ReducibleType >& reduction ) -> bool
				{
					return reduction.getCount() <= 0;
				});
			}
		};
	
	}

	template< typename... ReducibleType >
	class Reductions:
		private reductionsDetail::SpecificReductions< ReducibleType >...
	{
	public:
		template< typename T >
		std::function< T&() >
		prepareReduction( int mapCount, std::function< void( T ) > reductionOp )
		{
			return reductionsDetail::SpecificReductions< T >::prepareReduction( mapCount, reductionOp );
		}

		void
		check()
		{
			rod::generate
			{
				( reductionsDetail::SpecificReductions< ReducibleType >::check(), 0 )...
			};
		}
	};

}}
