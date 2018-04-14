
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/security/server_credentials.h>
#include "chat.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using chat::Chat;
using chat::Message;
using std::chrono::system_clock;

class ChatImpl final : public Chat::Service {
 public:
  explicit ChatImpl() {

  }

  Status Talk(ServerContext* context,
                   ServerReaderWriter<Message, Message>* stream) override {
    std::vector<Message> received_notes;
    Message note;
    while (stream->Read(&note)) {
      std::cout << "Receive message" << note.content() << std::endl;
      note.set_content(note.content() + " reply");
      stream->Write(note);
    }

    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  ChatImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}
