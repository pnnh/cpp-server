
find_package(Boost COMPONENTS system thread REQUIRED)
if (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    LINK_LIBRARIES(${Boost_LIBRARIES})
endif (Boost_FOUND)

add_executable(client client.cpp)
TARGET_INCLUDE_DIRECTORIES (client
        PRIVATE
        $<TARGET_PROPERTY:msgpackc,INTERFACE_INCLUDE_DIRECTORIES>
        )
