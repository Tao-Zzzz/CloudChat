#include "RPConPool.h"

RPConPool::RPConPool(size_t poolsize, std::string host, std::string port) :poolSize_(poolsize), host_(host), port_(port), b_stop_(false) {
	for (size_t i = 0; i < poolSize_; i++) {
		// Ϊÿ�����Ӵ�����ʹ
		std::shared_ptr<Channel> channel = grpc::CreateChannel(host_ + ":" + port_,
			grpc::InsecureChannelCredentials());

		// ���������ߵ����ƶ�����
		connections_.push(VarifyService::NewStub(channel));
	}
}

RPConPool::~RPConPool() {
	// �ȼ���,����ǰb_stop_ = false��Ҫ�������,������b_stop_=true ��������
	std::lock_guard<std::mutex> lock(mutex_);
	Close();
	while (!connections_.empty()) {
		connections_.pop();
		// ֪ͨ�ȴ�������߳̽���
	}
}

void RPConPool::Close() {
	b_stop_ = true;
	cond_.notify_all();
}

std::unique_ptr<VarifyService::Stub> RPConPool::getConnection() {
	std::unique_lock<std::mutex> lock(mutex_);
	// lamda���ʽ������,����������
	// ����false,�ѽ��̹���,�����ȱ���notify,�ż�������������
	cond_.wait(lock, [this]() {
		if (b_stop_) {
			return true;
		}
		// ���п�,����false,������
		// ���շ���true,��������ִ��
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
		// �Ѿ�ֹͣ��
		return;
	}

	connections_.push(std::move(context));
	// ����һ���߳�
	cond_.notify_one();
}
