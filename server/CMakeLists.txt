
find_package(Boost COMPONENTS system thread REQUIRED)
if (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    LINK_LIBRARIES(${Boost_LIBRARIES})
endif (Boost_FOUND)

find_package(msgpack REQUIRED)
if (msgpack_FOUND)
    INCLUDE_DIRECTORIES(${msgpack_INCLUDE_DIRS})
    LINK_LIBRARIES(${msgpack_LIBRARIES})
endif (msgpack_FOUND)

add_executable(server main.cpp server.cpp server.h)
#TARGET_INCLUDE_DIRECTORIES (server
#        PRIVATE
#        $<TARGET_PROPERTY:msgpackc,INTERFACE_INCLUDE_DIRECTORIES>
#        )