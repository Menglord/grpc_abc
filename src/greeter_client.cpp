#include <iostream>
#include <grpcpp/grpcpp.h>
#include <thread>
#include <functional>

#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

class Foo {
public:
	Foo(){}

	void Print(const std::string& str) {
		std::cout << "Foo Says: " << std::endl;
	}
};

class GreeterClient {
public:
	GreeterClient(std::shared_ptr<Channel> channel) : stub_(Greeter::NewStub(channel)) {}

	std::string SayHello(const std::string& user) {
		HelloRequest request;
		request.set_name(user);
		HelloReply reply;
		ClientContext context;
		Status status = stub_->SayHello(&context, request, &reply);
		if (status.ok()) {
			return reply.message();
		} else {
			std::cout << status.error_code() << ": " << status.error_message() << std::endl;
			return "RPC Failed";
		}
	}

	std::string SayHello(const std::string& user, std::function<void(std::string)> callback) {
		auto cb = callback;
		auto func = [cb, this, &user](){
			std::string reply = this->SayHello(user);
			if (cb) {
				cb(reply);
			}
		};
		std::thread t(func);
		t.detach();
	}

private:
	std::unique_ptr<Greeter::Stub> stub_;
};

void PrintReply(std::string reply) {
	std::cout << reply << std::endl;
}

int main(int argc, char** argv){
	GreeterClient greeter(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
	greeter.SayHello("forwardx", std::bind(PrintReply, std::placeholders::_1));
	std::cout << "ASYNC" << std::endl;
	int a;
	std::cin >> a;
	return 0;
}
