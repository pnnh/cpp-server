
ADD_LIBRARY(server SHARED SocketServer.h SocketServer.cpp)

find_package(Boost COMPONENTS system thread REQUIRED)
if (Boost_FOUND)
    include_directories(server ${Boost_INCLUDE_DIRS})
    target_link_libraries(server ${Boost_LIBRARIES})
endif (Boost_FOUND)

