#pragma once
#include <memory>
#include <string>
#include "MysqlPool.h"
#include "ConfigMgr.h"

class UserInfo {
public:
    std::string name;
    std::string pwd;
    int uid;
    std::string email;
};

class MysqlDao
{
public:
    MysqlDao();
    ~MysqlDao();
    int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
    bool UpdatePwd(const std::string& name, const std::string& newpwd);
    bool CheckEmail(const std::string& name, const std::string& email);
    bool CheckPwd(const std::string& email, const std::string& pwd, UserInfo& userInfo);

private:
    std::unique_ptr<MysqlPool> pool_;
};