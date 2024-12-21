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

    //建立表
    void createTable();

    //把某时间解析到某时期
    std::string parseTimeToPeriod(const std::string& time);

    //改变一个时间段内某个座位的占用状态
    void periodChangeStatus(std::string& period, std::string& computer_id, std::string& status);

    //改变多个时间段内某个座位的占用状态
    void ChangePeriodStatus(std::string& timeStart, std::string& timeLast, std::string& computer_id, std::string& status);

    //从时间段解析到多段时期
    Vector<std::string> generateTimeList(const std::string& start, const std::string& end);

    //重置全部座位记录
    void ResetAll();

    //返回某一时期内的电脑存量
    Vector<std::string> findAvaiComputer(std::string& time);

    //找到最近的电脑存量（单一时期）
    Vector<std::string> FindClosestSolution(const std::string& time);

    //找出这一段时期的电脑存量，使用取交集的办法
    Vector<std::string> FindPeriodSeatsAvai(const std::string& timeStart, const std::string& timeLast);

    //获取座位情况的二维数组
    Vector<Vector<std::string>> GetTableMap();

private:
    sqlite3pp::database db;
};

#endif
