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
    void creatTable();
    void RegisterStudent(std::string name, std::string phone, std::string passwd, std::string amout);
    void RechargeAmout(float money, const std::string& phone_number);
    void CostAmout(float money, const std::string& phone_number);
    Vector<std::string> GetInfo(std::string Phone);
    Vector<std::string> GetAmout(std::string Phone);

private:
    sqlite3pp::database db;
};

#endif