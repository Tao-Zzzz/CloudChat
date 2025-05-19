#pragma once

#include "const.h"

// 奇异递归模板类 enable_shared
class CServer:public std::enable_shared_from_this<CServer>
{
public:
	// 上下文和端口
	CServer(net::io_context& ioc, unsigned short& port);
	void Start();
private:
	// 要跟事件循环通信，上下文作为媒介绑定调度器
	tcp::acceptor _acceptor;


	net::io_context& _ioc; // 接收链接的ioc不变,处理链接的用连接池管理
	
	// 复用，接收对端的连接信息
	//tcp::socket _socket;

};

