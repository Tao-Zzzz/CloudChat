#pragma once

#include "const.h"
#include <grpcpp/grpcpp.h> 
#include "message.grpc.pb.h"
#include "message.pb.h"
#include <atomic>
#include <queue>

// ����ͨ��
using grpc::Channel;
// ���ʽ��
using grpc::Status;
// ͨ��������
using grpc::ClientContext;

using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::LoginRsp;
using message::LoginReq;
using message::StatusService;


// grpc���ӳأ���GRPConPoolһ��
class StatusConPool {
public:
	StatusConPool(size_t poolSize, std::string host, std::string port);

	~StatusConPool();

	std::unique_ptr<StatusService::Stub> getConnection();

	void returnConnection(std::unique_ptr<StatusService::Stub> context);

	void Close();

private:
	std::atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;
	std::queue<std::unique_ptr<StatusService::Stub>> connections_;
	std::mutex mutex_;
	std::condition_variable cond_;
};
