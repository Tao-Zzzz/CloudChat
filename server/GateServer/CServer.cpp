#include "CServer.h"
#include "HttpConnection.h"
#include "AsioIOContextPool.h"
// 类的成员是引用类型，要在初始化列表进行初始化
CServer::CServer(net::io_context& ioc, unsigned short& port):_ioc(ioc), 
_acceptor(ioc, tcp::endpoint(tcp::v4(),port)){

}

//旧版: 一个ioc既要处理监听又要处理创建
// 改成新版,	变成多线程,考虑资源互斥,线程挂起等
//void CServer::Start() {
//	// 共同管理一块内存，引用计数同步
//	auto self = shared_from_this();
//	_acceptor.async_accept(_socket, [self](beast::error_code ec) {
//		try {
//			// 出错则放弃链接，继续监听其他连接
//			if (ec) {
//				self->Start();
//				return;
//			}
//
//			/*
//				socket没有赋值，所以会用 move，根据_socket创建一个自己的socket，而且一般会使用智能指针
//				创建新连接,并且创建HttpConnection类管理这个连接
//			*/
//			std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
//
//			//继续监听
//
//			self->Start();
//		
//		}
//		catch (std::exception& exp) {
//
//		}
//		});
//}

//更新: 每一次进行监听的时候创建socket, 这个socket就用来链接,创建httpConnection
void CServer::Start() {
	// 共同管理一块内存，引用计数同步
	auto self = shared_from_this();

	// 从池子里取
	auto& io_context = AsioIOContextPool::GetInstance()->GetIOContext();

	// HttpConnection里会生成一个socket与ioc绑定起来
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);
	_acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code ec) {
		try {
			// 出错则放弃链接，继续监听其他连接
			if (ec) {
				self->Start();
				return;
			}

			// 捕获链接,直接启动
			new_con->Start();

			//继续监听

			self->Start();

		}
		catch (std::exception& exp) {

		}
	});
}