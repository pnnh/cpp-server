#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include "chat.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using chat::Chat;
using chat::Message;

Message MakeMessage(const std::string& content) {
  Message n;
  n.set_content(content);
  return n;
}

class ChatClient {
 public:
    ChatClient(std::shared_ptr<Channel> channel)
      : stub_(Chat::NewStub(channel)) {
  }

  void Talk() {
    ClientContext context;

    std::shared_ptr<ClientReaderWriter<Message, Message> > stream(
        stub_->Talk(&context));

    std::thread writer([stream]() {
      std::vector<Message> notes{
              MakeMessage("First message"),
              MakeMessage("Second message"),
              MakeMessage("Third message"),
              MakeMessage("Fourth message")};
      for (const Message& note : notes) {
        std::cout << "Sending message " << note.content() << std::endl;
        stream->Write(note);
      }
      stream->WritesDone();
    });

    Message server_note;
    while (stream->Read(&server_note)) {
      std::cout << "Got message " << server_note.content() << std::endl;
    }
    writer.join();
    Status status = stream->Finish();
    if (!status.ok()) {
      std::cout << "RouteChat rpc failed." << std::endl;
    }
  }

 private:
  std::unique_ptr<Chat::Stub> stub_;
};

int main(int argc, char** argv) {
  ChatClient guide(
      grpc::CreateChannel("localhost:50051",
                          grpc::InsecureChannelCredentials()));

  std::cout << "-------------- Talk --------------" << std::endl;
  guide.Talk();

  return 0;
}
