#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h" //�ⲿ���ǻ��� Protobuf �Զ����ɵĴ��룬������ VarifyService �������ص���Ϣ�ṹ
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



// ���ӱ���˶��߳�, ��ôstubҲҪ��ɶ��߳�(���ӳ�)����
class VerifyGrpcClient:public Singleton<VerifyGrpcClient>
{
	friend class Singleton<VerifyGrpcClient>;
public:
	GetVarifyRsp GetVarifyCode(std::string email); 
private:
	VerifyGrpcClient(); 
	//ͨ��stub���������ͨ��,��ʹ,unique_ptr����,������move
	std::unique_ptr<RPConPool> pool_;
};

 