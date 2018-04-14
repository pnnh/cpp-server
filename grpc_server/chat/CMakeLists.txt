
set(pbsrc ../pb/chat.pb.h ../pb/chat.pb.cc
        ../pb/chat.grpc.pb.h ../pb/chat.grpc.pb.cc)

add_executable(chat_server chat_server.cc
        ${pbsrc})

add_executable(chat_client chat_client.cc
        ${pbsrc})