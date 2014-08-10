#pragma once


#include <QIODevice>

#include <heads/common/Message.hpp>




namespace heads {
namespace common
{

	class ProtocolWriter
	{
	public:
		static void		write( QIODevice& writeDevice, const Message& message );
	};

}}
