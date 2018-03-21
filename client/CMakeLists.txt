
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

add_executable(client client.cpp)
#TARGET_INCLUDE_DIRECTORIES (client
#        PRIVATE
#        $<TARGET_PROPERTY:msgpackc,INTERFACE_INCLUDE_DIRECTORIES>
#        )
