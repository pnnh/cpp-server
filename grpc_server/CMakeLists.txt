
find_package(gRPC CONFIG REQUIRED)
link_libraries(gRPC::grpc++_unsecure)

include_directories(pb)

set(pbsrc pb/helloworld.pb.h pb/helloworld.pb.cc
        pb/helloworld.grpc.pb.h pb/helloworld.grpc.pb.cc)

add_executable(greeter_server
        greeter_server.cc
        ${pbsrc})

add_executable(greeter_client
        greeter_client.cc
        ${pbsrc})

set(routesrc pb/route_guide.pb.h pb/route_guide.pb.cc
        pb/route_guide.grpc.pb.h pb/route_guide.grpc.pb.cc)

# route_guide_server
add_executable(route_guide_server route/route_guide_server.cc route/helper.cc
        ${routesrc})

# route_guide_client
add_executable(route_guide_client route/route_guide_client.cc route/helper.cc
        ${routesrc})

configure_file(route/route_guide_db.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)