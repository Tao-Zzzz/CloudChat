#include "VerifyGrpcClient.h"
#include "ConfigMgr.h"
GetVarifyRsp VerifyGrpcClient::GetVarifyCode(std::string email)
{
	ClientContext context; // ���� gRPC ���õ���������Ϣ���糬ʱ���ã�
	GetVarifyRsp reply;
	GetVarifyReq request; // ����������������ַ
	request.set_email(email);

	// �����ӳ����ó�
	auto stub = pool_->getConnection();
	Status status = stub->GetVarifyCode(&context, request, &reply);
	if (status.ok()) {
		// ����ɹ������ط���˵���Ӧ
		pool_->returnConnection(std::move(stub));
		return reply;
	}
	else {
		// ����
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

