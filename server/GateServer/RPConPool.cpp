#include "RPConPool.h"

RPConPool::RPConPool(size_t poolsize, std::string host, std::string port) :poolSize_(poolsize), host_(host), port_(port), b_stop_(false) {
	for (size_t i = 0; i < poolSize_; i++) {
		// 为每个链接创建信使
		std::shared_ptr<Channel> channel = grpc::CreateChannel(host_ + ":" + port_,
			grpc::InsecureChannelCredentials());

		// 函数返回走的是移动构造
		connections_.push(VarifyService::NewStub(channel));
	}
}

RPConPool::~RPConPool() {
	// 先加锁,加锁前b_stop_ = false的要运行完毕,加锁后b_stop_=true 不会运行
	std::lock_guard<std::mutex> lock(mutex_);
	Close();
	while (!connections_.empty()) {
		connections_.pop();
		// 通知等待挂起的线程结束
	}
}

void RPConPool::Close() {
	b_stop_ = true;
	cond_.notify_all();
}

std::unique_ptr<VarifyService::Stub> RPConPool::getConnection() {
	std::unique_lock<std::mutex> lock(mutex_);
	// lamda表达式返回真,加锁往下走
	// 返回false,把进程挂起,解锁等别人notify,才继续加锁往下走
	cond_.wait(lock, [this]() {
		if (b_stop_) {
			return true;
		}
		// 队列空,返回false,被挂起
		// 不空返回true,继续往下执行
		return !connections_.empty();
		});

	if (b_stop_) {
		return nullptr;
	}
	// unique_ptr, move
	std::unique_ptr<VarifyService::Stub> context = std::move(connections_.front());
	connections_.pop();
	return context;
}

void RPConPool::returnConnection(std::unique_ptr<VarifyService::Stub> context) {
	std::lock_guard<std::mutex> lock(mutex_);
	if (b_stop_) {
		// 已经停止了
		return;
	}

	connections_.push(std::move(context));
	// 激活一个线程
	cond_.notify_one();
}
