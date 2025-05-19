#pragma once
#include "const.h"

class HttpConnection:public std::enable_shared_from_this<HttpConnection>
{
public:
	friend class LogicSystem;
	// 不接受socket而接收iocontext,而且要用引用的方式传递
	//HttpConnection(tcp::socket socket);
	HttpConnection(boost::asio::io_context& ioc);
	void Start();
	tcp::socket& GetSocket();
private:
	// 超时检测
	void CheckDeadline();
	// 应答
	void WriteResponse();
	// 处理请求
	void HandleReq();

	void PreParseGetParam();

	tcp::socket _socket;
	beast::flat_buffer _buffer{ 8192 };
	http::request<http::dynamic_body> _request;
	http::response<http::dynamic_body> _response;
	net::steady_timer deadline_{
		_socket.get_executor(), std::chrono::seconds(60)
	};
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;
};

