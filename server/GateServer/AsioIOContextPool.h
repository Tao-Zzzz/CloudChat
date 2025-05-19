#pragma once

#include <vector>
#include <boost/asio.hpp>
#include "Singleton.h"

class AsioIOContextPool:public Singleton<AsioIOContextPool>
{
	friend Singleton<AsioIOContextPool>;
public:
	using IOContext = boost::asio::io_context;
	// 用work绑定ioC可以保证ioC在没有任何事件的时候也不会退出，调用run时一直轮询着
	using Work = boost::asio::io_context::work;
	using WorkPtr = std::unique_ptr<Work>;
	~AsioIOContextPool();
	// 拷贝构造和拷贝赋值不让用
	AsioIOContextPool(const AsioIOContextPool&) = delete;
	AsioIOContextPool& operator=(const AsioIOContextPool&) = delete;

	IOContext& GetIOContext();
	void Stop(); // 回收资源
private:
	AsioIOContextPool(std::size_t size = 2); // 默认两个线程
	std::vector<IOContext> _ioContexts; // 存上下文，配套一个workptr和一个线程
	std::vector<WorkPtr> _works;
	std::vector<std::thread> _threads;
	std::size_t _nextIOContext; 
};

