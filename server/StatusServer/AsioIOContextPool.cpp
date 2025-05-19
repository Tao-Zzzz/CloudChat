#include "AsioIOContextPool.h"
#include <iostream>

AsioIOContextPool::~AsioIOContextPool(){
	Stop(); // �Լ�����
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
	// ����ִ��work.reset������iocontext�˳�run״̬
	// ��iocontext�Ѿ����˶���д�ļ����¼���,����Ҫ�ֶ�stop����
	for (auto& work : _works) {
		// �ѷ���ֹͣ��run�᷵��
		work->get_io_context().stop();
		// ָ���ɿ�ָ�룬ioContextҲ�����
		work.reset();
	}
	// �߳�ҲҪ�ȴ�������ϲŽ�������
	for (auto& t : _threads) {
		t.join();
	}
}

// io_context��֧�ֿ�������˷ŵ���ʼ���б�
AsioIOContextPool::AsioIOContextPool(std::size_t size) :_ioContexts(size),
_works(size), _nextIOContext(0) {
	// nextIOC ��0��ʼȡ
	for (std::size_t i = 0; i < size; i++) {
		// �����µİ���ioc��work��iocû�п���������make_unique
		_works[i] = std::unique_ptr<Work>(new Work(_ioContexts[i]));
	}

	// ����ioc����������̣߳�ÿ���߳��ڲ�����ioc.run
	for (std::size_t i = 0; i < _ioContexts.size(); i++) {
		_threads.emplace_back([this, i]() {
			// ��Ϊ����work�����Բ��᷵��
			_ioContexts[i].run();
			});
	}
}
