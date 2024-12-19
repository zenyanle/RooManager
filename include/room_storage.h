#ifndef ROOM_STORAGE_H
#define ROOM_STORAGE_H

//#include <string>
//#include <vector>
#include "sqlite3pp.h"
#include "my_vector.h"

class RoomStorage {
public:
    RoomStorage(const std::string& db_name);
    ~RoomStorage();

    void createTable();
    std::string parseTimeToPeriod(const std::string& time);
    void periodChangeStatus(std::string& period, std::string& computer_id, std::string& status);
    void ChangePeriodStatus(std::string& timeStart, std::string& timeLast, std::string& computer_id, std::string& status);
    Vector<std::string> generateTimeList(const std::string& start, const std::string& end);
    void ResetAll();
    Vector<std::string> findAvaiComputer(std::string& time);
    Vector<std::string> FindClosestSolution(const std::string& time);
    Vector<std::string> FindPeriodSeatsAvai(const std::string& timeStart, const std::string& timeLast);
    Vector<Vector<std::string>> GetTableMap();

private:
    sqlite3pp::database db;
};

#endif
