#pragma once

#include <vector>
#include <boost/asio.hpp>
#include "Singleton.h"

class AsioIOContextPool:public Singleton<AsioIOContextPool>
{
	friend Singleton<AsioIOContextPool>;
public:
	using IOContext = boost::asio::io_context;
	// ��work��ioC���Ա�֤ioC��û���κ��¼���ʱ��Ҳ�����˳�������runʱһֱ��ѯ��
	using Work = boost::asio::io_context::work;
	using WorkPtr = std::unique_ptr<Work>;
	~AsioIOContextPool();
	// ��������Ϳ�����ֵ������
	AsioIOContextPool(const AsioIOContextPool&) = delete;
	AsioIOContextPool& operator=(const AsioIOContextPool&) = delete;

	IOContext& GetIOContext();
	void Stop(); // ������Դ
private:
	AsioIOContextPool(std::size_t size = 2); // Ĭ�������߳�
	std::vector<IOContext> _ioContexts; // �������ģ�����һ��workptr��һ���߳�
	std::vector<WorkPtr> _works;
	std::vector<std::thread> _threads;
	std::size_t _nextIOContext; 
};

