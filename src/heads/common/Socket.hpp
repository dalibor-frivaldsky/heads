#pragma once


#include <memory>

#include <QLocalSocket>




namespace heads {
namespace common
{

	using Socket = std::unique_ptr< QLocalSocket >;

}}
