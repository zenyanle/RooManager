#ifndef STUDENT_STORAGE_H
#define STUDENT_STORAGE_H

//#include <string>
//#include <vector>
#include "sqlite3pp.h"
#include "my_vector.h"

class StudentStorage {
public:

    StudentStorage(const std::string& db_name);
    ~StudentStorage();

    //建立表
    void creatTable();

    //注册学生
    void RegisterStudent(std::string name, std::string phone, std::string passwd, std::string amout);

    //学生充值
    void RechargeAmout(float money, const std::string& phone_number);

    //消耗余额
    void CostAmout(float money, const std::string& phone_number);

    //获取学生所有数据的统一方法
    Vector<std::string> GetInfo(std::string Phone);

private:
    sqlite3pp::database db;
};

#endif