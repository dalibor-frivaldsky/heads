INCLUDEPATH += $$PWD/src

SOURCES += $$PWD/src/heads/Bootstrapper.cpp \
	$$PWD/src/heads/common/Connection.cpp \
	$$PWD/src/heads/common/HeadId.cpp \
	$$PWD/src/heads/common/Message.cpp \
	$$PWD/src/heads/common/ProtocolReader.cpp \
	$$PWD/src/heads/common/ProtocolWriter.cpp \
	$$PWD/src/heads/common/QueryId.cpp \
	$$PWD/src/heads/common/QueryIdProvider.cpp \
	$$PWD/src/heads/common/Request.cpp \
	$$PWD/src/heads/common/RequestPool.cpp \
	$$PWD/src/heads/common/Response.cpp \
	$$PWD/src/heads/common/ServerNaming.cpp \
	$$PWD/src/heads/head/HeadServer.cpp \
	$$PWD/src/heads/head/RootConnection.cpp \
	$$PWD/src/heads/root/RootServer.cpp \
	$$PWD/src/heads/root/UnpairedSockets.cpp

HEADERS += $$PWD/src/heads/root/annotation/ListenersAccessor.hpp \
	$$PWD/src/heads/root/HeadListenersAccessor.hpp \
	$$PWD/src/heads/InformOthers.hpp \
	$$PWD/src/heads/Bootstrapper.hpp \
	$$PWD/src/heads/Heads.hpp \
	$$PWD/src/heads/Inform.hpp \
	$$PWD/src/heads/MessageControllers.hpp \
	$$PWD/src/heads/Query.hpp \
	$$PWD/src/heads/Reduce.hpp \
	$$PWD/src/heads/Reducibles.hpp \
	$$PWD/src/heads/Reply.hpp \
	$$PWD/src/heads/Topic.hpp \
	$$PWD/src/heads/annotation/HeadDescriptor.hpp \
	$$PWD/src/heads/annotation/MessageController.hpp \
	$$PWD/src/heads/annotation/RootDescriptor.hpp \
	$$PWD/src/heads/common/Connection.hpp \
	$$PWD/src/heads/common/HeadId.hpp \
	$$PWD/src/heads/common/Message.hpp \
	$$PWD/src/heads/common/MessageDispatcher.hpp \
	$$PWD/src/heads/common/ProtocolReader.hpp \
	$$PWD/src/heads/common/ProtocolWriter.hpp \
	$$PWD/src/heads/common/QueryId.hpp \
	$$PWD/src/heads/common/QueryIdProvider.hpp \
	$$PWD/src/heads/common/Request.hpp \
	$$PWD/src/heads/common/RequestPool.hpp \
	$$PWD/src/heads/common/Response.hpp \
	$$PWD/src/heads/common/ServerNaming.hpp \
	$$PWD/src/heads/common/Socket.hpp \
	$$PWD/src/heads/head/Head.hpp \
	$$PWD/src/heads/head/HeadServer.hpp \
	$$PWD/src/heads/head/RootConnection.hpp \
	$$PWD/src/heads/root/ConnectionPreRegister.hpp \
	$$PWD/src/heads/root/ConnectionRegisterer.hpp \
	$$PWD/src/heads/root/HeadListener.hpp \
	$$PWD/src/heads/root/HeadListeners.hpp \
	$$PWD/src/heads/root/HeadReduceService.hpp \
	$$PWD/src/heads/root/Reducible.hpp \
	$$PWD/src/heads/root/RegisterMessage.hpp \
	$$PWD/src/heads/root/Root.hpp \
	$$PWD/src/heads/root/RootServer.hpp \
	$$PWD/src/heads/root/UnpairedSockets.hpp \
	$$PWD/src/heads/root/annotation/ReduceService.hpp \
	$$PWD/src/heads/root/annotation/ReducibleDescriptor.hpp