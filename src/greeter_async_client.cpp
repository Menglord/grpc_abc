#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>

#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

class GreeterClient {
public:
	explicit GreeterClient(std::shared_ptr<Channel> channel) : stub_(Greeter::NewStub(channel)) {}
	std::string SayHello(const std::string& user) {
		HelloRequest request;
		request.set_name(user);
		HelloReply reply;
		ClientContext context;
		CompletionQueue cq;
		Status status;
		std::unique_ptr<ClientAsyncResponseReader<HelloReply>> rpc(stub_->PrepareAsyncSayHello(&context, request, &cq));
		rpc->StartCall();
		rpc->Finish(&reply, &status, (void*)1);
		void* got_tag;
		bool ok = false;
		GPR_ASSERT(cq.Next(&got_tag, &ok));
		GPR_ASSERT(got_tag == (void*)1);
		GPR_ASSERT(ok);
		if (status.ok()) {
			return reply.message();
		} else {
			return "RPC Failed";
		}
	}

private:
	std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
	std::string user("world");
	std::thread t[5];
	for (int i = 0; i < 5; ++i) {
		t[i] = std::thread([](){
			GreeterClient greeter(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
			greeter.SayHello("world");
		});
	}
	for (int i = 0; i < 5; ++i) {
		t[i].join();
	}
	return 0;
}
