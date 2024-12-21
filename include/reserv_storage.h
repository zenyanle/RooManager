#ifndef RESERV_STORAGE_H
#define RESERV_STORAGE_H


//#include <string>
//#include <vector>
#include "sqlite3pp.h"
#include "my_vector.h"
#include "hash_table.h"

class ReservStorage {
public:

    ReservStorage(const std::string& db_name);
    ~ReservStorage();

    //建立表
    void creatTable();

    //添加预约记录
    void addRecord(int status, const std::string& phone_number, int computer_id, int is_completed, const std::string& start_time, const std::string& end_time, const std::string& fullCurrentTime);
    
    //记录生命周期结束，添加完成tag
    void finishRecord(const std::string& phone_number, const std::string& fullCurrentTime);
    
    //获取学生预约信息
    Vector<std::string> GetPeriod(const std::string& phone_number);

    //获取当前还未完成的记录
    Vector<Vector<std::string>> GetActiveRecord();

    //添加预约等待记录，status为0
    void AddStatusOneRecord(const std::string& phone_number);

    //获取等待记录
    Vector<std::string> GetWaitingRecords();

    //哈希表关联方法，获取每个学生进入机房的次数
    void SumStudents(HashTable& phoneBook);



private:
    sqlite3pp::database db;
};

#endif