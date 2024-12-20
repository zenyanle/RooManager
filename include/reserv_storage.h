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
    void creatTable();
    void addRecord(int status, const std::string& phone_number, int computer_id, int is_completed, const std::string& start_time, const std::string& end_time, const std::string& fullCurrentTime);
    void finishRecord(const std::string& phone_number, const std::string& fullCurrentTime);
    Vector<std::string> GetPeriod(const std::string& phone_number);
    Vector<Vector<std::string>> GetActiveRecord();
    void AddStatusOneRecord(const std::string& phone_number);
    Vector<std::string> GetWaitingRecords();
    void SumStudents(HashTable& phoneBook);



private:
    sqlite3pp::database db;
};

#endif