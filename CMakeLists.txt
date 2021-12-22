cmake_minimum_required(VERSION 3.5)

project(cpp-server)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_PREFIX_PATH "/usr/local/Cellar")

find_package(PkgConfig REQUIRED)

find_package(Boost COMPONENTS system thread REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_LIBRARIES(${Boost_LIBRARIES})

find_package(folly REQUIRED)
INCLUDE_DIRECTORIES(${folly_INCLUDE_DIRS})
LINK_LIBRARIES(${folly_LIBRARIES})

pkg_search_module(OPENSSL REQUIRED openssl)
#find_package(OpenSSL REQUIRED)
#include_directories(${OPENSSL_INCLUDE_DIR})
#LINK_LIBRARIES(${OPENSSL_LIBRARIES})

add_subdirectory(src)
