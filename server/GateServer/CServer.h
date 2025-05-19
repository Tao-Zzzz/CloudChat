#pragma once

#include "const.h"

// ����ݹ�ģ���� enable_shared
class CServer:public std::enable_shared_from_this<CServer>
{
public:
	// �����ĺͶ˿�
	CServer(net::io_context& ioc, unsigned short& port);
	void Start();
private:
	// Ҫ���¼�ѭ��ͨ�ţ���������Ϊý��󶨵�����
	tcp::acceptor _acceptor;


	net::io_context& _ioc; // �������ӵ�ioc����,�������ӵ������ӳع���
	
	// ���ã����նԶ˵�������Ϣ
	//tcp::socket _socket;

};

