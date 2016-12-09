cmake_minimum_required(VERSION 3.5)
project(cppdemo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread")

ADD_SUBDIRECTORY(server)

add_executable(cppdemo main.cpp)
TARGET_LINK_LIBRARIES(cppdemo server)
