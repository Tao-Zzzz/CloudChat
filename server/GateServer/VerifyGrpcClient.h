#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h" //这部分是基于 Protobuf 自动生成的代码，定义了 VarifyService 服务和相关的消息结构
#include "const.h"
#include "Singleton.h"
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "RPConPool.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;



// 链接变成了多线程, 那么stub也要变成多线程(连接池)才行
class VerifyGrpcClient:public Singleton<VerifyGrpcClient>
{
	friend class Singleton<VerifyGrpcClient>;
public:
	GetVarifyRsp GetVarifyCode(std::string email); 
private:
	VerifyGrpcClient(); 
	//通过stub才能与别人通信,信使,unique_ptr类型,可以用move
	std::unique_ptr<RPConPool> pool_;
};

 