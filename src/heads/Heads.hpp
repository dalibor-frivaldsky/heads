#pragma once


#include <rod/Contextual.hpp>
#include <rod/factory/FactoryResolver.hpp>

#include <heads/common/ServerNaming.hpp>
#include <heads/common/Socket.hpp>
#include <heads/head/Head.hpp>
#include <heads/root/Root.hpp>




namespace heads {

	template< typename Context >
	class Heads:
		public rod::Contextual<
				Context,
				rod::factory::FactoryResolver,
				rod::AsSingleton< common::ServerNaming > >
	{
	public:
		ROD_Contextual_Constructor( Heads );


		void
		enter()
		{
			common::Socket	rootSocket = establishRootConnection();

			if( rootSocket->state() == QLocalSocket::UnconnectedState )
			{
				rod::create< root::Root >( this ).enter();
			}
			else
			{
				auto head = rod::create< head::Head >( this );
				head.setRootSocket( std::move( rootSocket ) );
				head.enter();
			}
		}


	private:
		common::Socket
		establishRootConnection()
		{
			common::Socket	rootSocket( new QLocalSocket() );

			rootSocket->connectToServer( rod::resolve< common::ServerNaming& >( this ).rootName(), QIODevice::WriteOnly );

			return std::move( rootSocket );
		}
	};

}