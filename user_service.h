#ifndef __USER_SERVICE_H_
#define __USER_SERVICE_H_



//操作数据库的模块
// 
//这个类实现了用户服务模块，包含了用户注册，用户登录，用户信息查询，用户信息修改等功能，
//和用户相关的事件处理函数，用户服务模块会调用sqlconnection模块来操作数据库，




#include <memory>
#include "sqlconnection.h"

class UserService
{
public:
	UserService(std::shared_ptr<MysqlConnection> sql_conn);

	bool exist(std::string& userName);
	bool verifyUserInfo(std::string& userName, std::string& userPwd);
	bool insert(std::string& userName, std::string& userPwd);
	bool addBike(i32 bikeid, r64 longitude, r64 latitude);
	bool deleteBike(i32 bikeid);
	bool updataBikeinfo(char* sql_content);
	bool insertUseRideRecord(char* sql_content);
	bool userRecordModify(char* sql_content);
	bool findBikeInfo(char* sql_content, SqlRecordSet &record_set);
	bool findUserInfo(char* sql_content, SqlRecordSet& record_set);
private:
	std::shared_ptr<MysqlConnection> sql_conn_;

};

#endif // !__USER_SERVICE_H_




