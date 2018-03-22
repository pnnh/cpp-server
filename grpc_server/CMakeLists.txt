
#find_package(Protobuf REQUIRED)
#if (Protobuf_FOUND)
#    INCLUDE_DIRECTORIES(${Protobuf_INCLUDE_DIRS})
#    LINK_LIBRARIES(${Protobuf_LIBRARIES})
#endif (Protobuf_FOUND)

find_package(gRPC CONFIG REQUIRED)
link_libraries(gRPC::grpc++_unsecure)

include_directories(pb)

set(pbsrc pb/helloworld.pb.cc pb/helloworld.grpc.pb.cc)

add_executable(greeter_server
        greeter_server.cc
        ${pbsrc})

add_executable(greeter_client
        greeter_client.cc
        ${pbsrc})
