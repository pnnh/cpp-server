cmake_minimum_required(VERSION 3.5)
project(cppdemo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp )
add_executable(cppdemo ${SOURCE_FILES})

find_package(Boost COMPONENTS system REQUIRED)
target_link_libraries(cppdemo ${Boost_LIBRARIES})