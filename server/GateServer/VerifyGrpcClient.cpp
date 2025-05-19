#include "VerifyGrpcClient.h"
#include "ConfigMgr.h"
GetVarifyRsp VerifyGrpcClient::GetVarifyCode(std::string email)
{
	ClientContext context; // 管理 gRPC 调用的上下文信息（如超时设置）
	GetVarifyRsp reply;
	GetVarifyReq request; // 请求对象，设置邮箱地址
	request.set_email(email);

	// 从链接池中拿出
	auto stub = pool_->getConnection();
	Status status = stub->GetVarifyCode(&context, request, &reply);
	if (status.ok()) {
		// 如果成功，返回服务端的响应
		pool_->returnConnection(std::move(stub));
		return reply;
	}
	else {
		// 回收
		pool_->returnConnection(std::move(stub));

		reply.set_error(ErrorCodes::RPCFailed);
		return reply;
	}
}

VerifyGrpcClient::VerifyGrpcClient()
{
	auto& gCfgMgr = ConfigMgr::Inst();
	std::string host = gCfgMgr["VarifyServer"]["Host"];
	std::string port = gCfgMgr["VarifyServer"]["Port"];
	pool_.reset(new RPConPool(5, host, port));

}

