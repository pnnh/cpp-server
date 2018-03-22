cmake_minimum_required(VERSION 3.5)

project(cppdemo)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread")



option(server "build server" ON)
option(grpc_server "build grpc_server" ON)

if(server)
    ADD_SUBDIRECTORY(server)
endif(server)

if(grpc_server)
    ADD_SUBDIRECTORY(grpc_server)
endif(grpc_server)

