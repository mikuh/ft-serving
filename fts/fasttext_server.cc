#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <fasttext/fasttext.h>

#ifdef BAZEL_BUILD
#include "protos/fts.grpc.pb.h"
#else
#include "fts.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using fts::PredictRequest;
using fts::PredictReply;
using fts::FastText;

fasttext::FastText ft;

// Logic and data behind the server's behavior.
class FastTextServiceImpl final : public FastText::Service {
  Status Predict(ServerContext* context, const PredictRequest* request,
                  PredictReply* reply) override {
    std::vector<std::pair<float, std::string>> predictions;
    std::istringstream in(request->text());
    ft.predictLine(in, predictions, 1, 0.0);
    if(!predictions.empty()){
      reply->set_label(predictions[0].second);
      reply->set_score(predictions[0].first);
    }
    // reply->set_label("Politics");
    // reply->set_score(1.1);
    return Status::OK;
  }
};

void RunServer() {

  ft.loadModel("/home/geb/PycharmProjects/auxiliary-discriminator/serving/models/model-128.bin");

  std::string server_address("0.0.0.0:50052");
  FastTextServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}
