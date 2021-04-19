#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "protos/fts.grpc.pb.h"
#else
#include "fts.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using fts::PredictRequest;
using fts::PredictReply;
using fts::FastText;

class FastTextClient {
 public:
  FastTextClient(std::shared_ptr<Channel> channel)
      : stub_(FastText::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::pair<std::string, float> Predict(const std::string& text) {
    // Data we are sending to the server.
    PredictRequest request;
    std::cout << text << std::endl;
    request.set_text(text);

    // Container for the data we expect from the server.
    PredictReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->Predict(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return std::make_pair(reply.label(), reply.score());
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return std::make_pair("RPC failed", -1.1);
    }
  }

 private:
  std::unique_ptr<FastText::Stub> stub_;
};

int main(int argc, char** argv) {

  std::string target_str = "localhost:50052";
  
  FastTextClient ftclient(grpc::CreateChannel(
      target_str, grpc::InsecureChannelCredentials()));
  std::string text("习近平 毛泽东 123");
  std::pair<std::string, float> reply = ftclient.Predict(text);
  std::cout << "ftc received: " << reply.first << " " << reply.second << std::endl;

  return 0;
}
