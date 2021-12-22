cmake_minimum_required(VERSION 3.5)

project(cpp-server)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)

find_package(Boost COMPONENTS system thread REQUIRED)
if (Boost_FOUND)
    message(STATUS "Found Boost: ${FOLLY_LIBRARIES}")
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    LINK_LIBRARIES(${Boost_LIBRARIES})
endif (Boost_FOUND)

find_package(folly REQUIRED)
if (folly_FOUND)
    message(STATUS "Found folly: ${FOLLY_LIBRARIES}")
    INCLUDE_DIRECTORIES(${folly_INCLUDE_DIRS})
    LINK_LIBRARIES(${FOLLY_LIBRARIES})
endif (folly_FOUND)

add_executable(server
    src/main.cc 
    src/http_connection.cc 
    src/state.cc 
    src/md5.cc)