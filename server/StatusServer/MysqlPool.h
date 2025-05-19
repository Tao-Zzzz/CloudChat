#pragma once
#include <string>
#include <memory>
#include <thread>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/exception.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Defer.h"

// �Լ���װ����mysql
// Ϊ��ʵ�ֿ������ӵ�ʱ��
class SqlConnection {
public:
	SqlConnection(sql::Connection* con, int64_t lasttime) :_con(con), _last_oper_time(lasttime) {}
	std::unique_ptr<sql::Connection> _con; // ʵ�ʵ�mysql���ӣ�������uniqueptr����Ҫ���б��й���
	int64_t _last_oper_time;
};

class MysqlPool {
public:
	MysqlPool(const std::string& url, const std::string& user, const std::string& pass, const std::string& schema, int poolSize);

	void checkConnection();
	std::unique_ptr<SqlConnection> getConnection();
	void returnConnection(std::unique_ptr<SqlConnection> con);

	void Close();

	~MysqlPool();

private:
	std::string url_;
	std::string user_;
	std::string pass_;
	std::string schema_;
	int poolSize_;
	std::queue<std::unique_ptr<SqlConnection>> pool_;
	std::mutex mutex_;
	std::condition_variable cond_;
	std::atomic<bool> b_stop_;
	std::thread _check_thread;
};