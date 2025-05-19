#include "AsioIOContextPool.h"
#include <iostream>

AsioIOContextPool::~AsioIOContextPool(){
	Stop(); // 自己回收
	std::cout << "AsioIOContextPool destruct" << std::endl;
}

boost::asio::io_context& AsioIOContextPool::GetIOContext(){
	auto& context = _ioContexts[_nextIOContext++];
	if (_nextIOContext == _ioContexts.size()) {
		_nextIOContext = 0;
	}
	return context;
}

void AsioIOContextPool::Stop(){
	// 仅仅执行work.reset不能让iocontext退出run状态
	// 当iocontext已经绑定了读或写的监听事件后,还需要手动stop服务
	for (auto& work : _works) {
		// 把服务停止，run会返回
		work->get_io_context().stop();
		// 指针变成空指针，ioContext也会回收
		work.reset();
	}
	// 线程也要等待运行完毕才进行析构
	for (auto& t : _threads) {
		t.join();
	}
}

// io_context不支持拷贝，因此放到初始化列表
AsioIOContextPool::AsioIOContextPool(std::size_t size) :_ioContexts(size),
_works(size), _nextIOContext(0) {
	// nextIOC 从0开始取
	for (std::size_t i = 0; i < size; i++) {
		// 创建新的绑定了ioc的work，ioc没有拷贝不能用make_unique
		_works[i] = std::unique_ptr<Work>(new Work(_ioContexts[i]));
	}

	// 遍历ioc，创建多个线程，每个线程内部启动ioc.run
	for (std::size_t i = 0; i < _ioContexts.size(); i++) {
		_threads.emplace_back([this, i]() {
			// 因为绑定了work，所以不会返回
			_ioContexts[i].run();
			});
	}
}
