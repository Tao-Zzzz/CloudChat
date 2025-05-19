#include "CServer.h"
#include "HttpConnection.h"
#include "AsioIOContextPool.h"
// ��ĳ�Ա���������ͣ�Ҫ�ڳ�ʼ���б���г�ʼ��
CServer::CServer(net::io_context& ioc, unsigned short& port):_ioc(ioc), 
_acceptor(ioc, tcp::endpoint(tcp::v4(),port)){

}

//�ɰ�: һ��ioc��Ҫ���������Ҫ������
// �ĳ��°�,	��ɶ��߳�,������Դ����,�̹߳����
//void CServer::Start() {
//	// ��ͬ����һ���ڴ棬���ü���ͬ��
//	auto self = shared_from_this();
//	_acceptor.async_accept(_socket, [self](beast::error_code ec) {
//		try {
//			// ������������ӣ�����������������
//			if (ec) {
//				self->Start();
//				return;
//			}
//
//			/*
//				socketû�и�ֵ�����Ի��� move������_socket����һ���Լ���socket������һ���ʹ������ָ��
//				����������,���Ҵ���HttpConnection������������
//			*/
//			std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
//
//			//��������
//
//			self->Start();
//		
//		}
//		catch (std::exception& exp) {
//
//		}
//		});
//}

//����: ÿһ�ν��м�����ʱ�򴴽�socket, ���socket����������,����httpConnection
void CServer::Start() {
	// ��ͬ����һ���ڴ棬���ü���ͬ��
	auto self = shared_from_this();

	// �ӳ�����ȡ
	auto& io_context = AsioIOContextPool::GetInstance()->GetIOContext();

	// HttpConnection�������һ��socket��ioc������
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);
	_acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code ec) {
		try {
			// ������������ӣ�����������������
			if (ec) {
				self->Start();
				return;
			}

			// ��������,ֱ������
			new_con->Start();

			//��������

			self->Start();

		}
		catch (std::exception& exp) {

		}
	});
}