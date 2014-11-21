#pragma once


#include <memory>

#include <QLocalSocket>
#include <QMetaType>




namespace heads {
namespace common
{

	using Socket = std::unique_ptr< QLocalSocket >;

}}

Q_DECLARE_METATYPE( QLocalSocket::LocalSocketError )
