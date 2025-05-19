#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <queue>
#include <mutex>
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class RPConPool {
public:
	RPConPool(size_t poolsize, std::string host, std::string port);
	~RPConPool();

	std::unique_ptr<VarifyService::Stub> getConnection();
	void returnConnection(std::unique_ptr<VarifyService::Stub> context);

	void Close();
private:
	std::atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;

	// queue �����Ż�,�������
	std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
	// �ײ������ʵ�ֵģ���
	std::condition_variable cond_;
	std::mutex mutex_;
};
