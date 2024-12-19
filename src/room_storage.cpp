#include "room_storage.h"
#include <iostream>
//#include "sqlite3pp.h"
#include <string>
#include <stdexcept>
#include <sstream>

RoomStorage::RoomStorage(const std::string& db_name) 
    : db(db_name.c_str()) {
        
    std::cout << "Database connected: " << db_name << std::endl;
}

RoomStorage::~RoomStorage() {
    std::cout << "Database closed" << std::endl;
}

void RoomStorage::createTable() {
    db.execute("CREATE TABLE IF NOT EXISTS computer_usage1 ( \
    computer_id INTEGER PRIMARY KEY AUTOINCREMENT,  -- 表示电脑编号 \
    time_0 INTEGER NOT NULL DEFAULT 0,             -- 每个时间段用整数值表示 \
    time_1 INTEGER NOT NULL DEFAULT 0, \
    time_2 INTEGER NOT NULL DEFAULT 0, \
    time_3 INTEGER NOT NULL DEFAULT 0, \
    time_4 INTEGER NOT NULL DEFAULT 0, \
    time_5 INTEGER NOT NULL DEFAULT 0, \
    time_6 INTEGER NOT NULL DEFAULT 0, \
    time_7 INTEGER NOT NULL DEFAULT 0, \
    time_8 INTEGER NOT NULL DEFAULT 0, \
    time_9 INTEGER NOT NULL DEFAULT 0, \
    time_10 INTEGER NOT NULL DEFAULT 0, \
    time_11 INTEGER NOT NULL DEFAULT 0, \
    time_12 INTEGER NOT NULL DEFAULT 0, \
    time_13 INTEGER NOT NULL DEFAULT 0, \
    time_14 INTEGER NOT NULL DEFAULT 0, \
    time_15 INTEGER NOT NULL DEFAULT 0, \
    time_16 INTEGER NOT NULL DEFAULT 0, \
    time_17 INTEGER NOT NULL DEFAULT 0, \
    time_18 INTEGER NOT NULL DEFAULT 0, \
    time_19 INTEGER NOT NULL DEFAULT 0, \
    time_20 INTEGER NOT NULL DEFAULT 0, \
    time_21 INTEGER NOT NULL DEFAULT 0, \
    time_22 INTEGER NOT NULL DEFAULT 0, \
    time_23 INTEGER NOT NULL DEFAULT 0 \
);");

}

//批量映射，调用parseTimeToPeriod
Vector<std::string> RoomStorage::generateTimeList(const std::string& start, const std::string& end) {
    Vector<std::string> timeList;

    // 提取起始和结束时间的小时和分钟部分
    int startHour, startMinute, endHour, endMinute;
    char colon; // 用于提取分隔符 ':'

    std::istringstream(start) >> startHour >> colon >> startMinute; // 解析起始时间
    std::istringstream(end) >> endHour >> colon >> endMinute; // 解析结束时间

    // 检查时间范围是否合法
    if (startHour < 0 || startHour > 23 || endHour < 0 || endHour > 23 ||
        startMinute < 0 || startMinute > 59 || endMinute < 0 || endMinute > 59) {
        throw std::invalid_argument("Time values must be valid.");
    }
    if (startHour > endHour || (startHour == endHour && startMinute > endMinute)) {
        throw std::invalid_argument("Start time must be less than or equal to end time.");
    }

    // 生成时间段列表
    for (int hour = startHour; hour <= endHour; ++hour) {
        for (int minute = (hour == startHour ? startMinute : 0); 
             minute < (hour == endHour ? endMinute + 1 : 60); 
             minute += 30) {
            // 处理分钟为整点和半点
            std::ostringstream timeStream;
            timeStream << hour << ":" << (minute < 10 ? "0" : "") << minute;
            timeList.push_back(timeStream.str());
        }
    }
    timeList.pop_back();

    return timeList;
}

//解析器，将“8:30”转成“time_2”，映射器
std::string RoomStorage::parseTimeToPeriod(const std::string& time) {
    std::string period;
    size_t colonPos = time.find(':');

    if (colonPos != std::string::npos) {
        // 提取小时和分钟部分
        std::string hourPart = time.substr(0, colonPos);   // 提取小时部分
        std::string minutePart = time.substr(colonPos + 1); // 提取分钟部分

        try {
            // 将字符串转换为整数
            int hour = std::stoi(hourPart);
            int minutes = std::stoi(minutePart);

            // 计算时间段
            if (minutes == 0) {
                period = "time_" + std::to_string((hour - 8) * 2);
            } else {
                period = "time_" + std::to_string((hour - 8) * 2 + 1);
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid time format: " << time << std::endl;
            throw;  // 抛出异常以便调用方处理
        } catch (const std::out_of_range& e) {
            std::cerr << "Time value out of range: " << time << std::endl;
            throw;  // 抛出异常以便调用方处理
        }
    } else {
        std::cerr << "Time string is in incorrect format. Expected HH:MM, got: " << time << std::endl;
        throw std::invalid_argument("Incorrect time format");
    }

    return period;
}

//改变某一时期的某个电脑状态
void RoomStorage::periodChangeStatus(std::string& period, std::string& computer_id, std::string& status) {
    std::string insertSQL = "UPDATE computer_usage1 SET " + period + " = " + status + " WHERE computer_id = " + computer_id + ";";
    //std::cout << "ping";
    db.execute(insertSQL.c_str());
}

//改变某一段时期的某个电脑的状态，上机下机通用
void RoomStorage::ChangePeriodStatus(std::string& timeStart, std::string& timeLast, std::string& computer_id, std::string& status) {
    Vector<std::string> times = generateTimeList(timeStart, timeLast);
    //Vector<std::string> times = generateTimeList(periodStart, periodLast);
    for (size_t i = 0; i < times.size(); ++i) {
        //std::cout << "ping" << std::endl;
        std::string period = parseTimeToPeriod(times[i]);
        periodChangeStatus(period, computer_id, status);
    }
}

//清空全部数据
void RoomStorage::ResetAll() {
    std::string resetSql = R"(
    UPDATE computer_usage1
    SET 
        time_0 = 0,
        time_1 = 0,
        time_2 = 0,
        time_3 = 0,
        time_4 = 0,
        time_5 = 0,
        time_6 = 0,
        time_7 = 0,
        time_8 = 0,
        time_9 = 0,
        time_10 = 0,
        time_11 = 0,
        time_12 = 0,
        time_13 = 0,
        time_14 = 0,
        time_15 = 0,
        time_16 = 0,
        time_17 = 0,
        time_18 = 0,
        time_19 = 0,
        time_20 = 0,
        time_21 = 0,
        time_22 = 0,
        time_23 = 0
; 
)";
    //std::cout << "ping";
    db.execute(resetSql.c_str());

}

//返回某一时期内的电脑存量
Vector<std::string> RoomStorage::findAvaiComputer(std::string& time) {
    Vector<std::string> list;
    std::string period = parseTimeToPeriod(time);
    std::string findAvaiComputerSQL = "SELECT computer_id FROM computer_usage1 WHERE " + period + " = 0;";
    sqlite3pp::query qry(db, findAvaiComputerSQL.c_str());
    for (auto v : qry) {
        std::string computer_id;
        v.getter() >> computer_id;
        //cout << "\t" << name << "\t" << phone << endl;
        list.push_back(computer_id);
    }
    return list;
}

//找到最近的电脑存量（单一时期）
Vector<std::string> RoomStorage::FindClosestSolution(const std::string& time) {
    Vector<std::string> list;
    Vector<std::string> times = generateTimeList(time, "20:00");
    for (size_t i = 0; i < times.size(); ++i) {
        Vector<std::string> seats = findAvaiComputer(times[i]);
        if (seats.empty()) {
            continue;
        }
        list.push_back(times[i]);
        for (size_t j = 0; j < seats.size(); ++j) {
            list.push_back(seats[j]);
        }
        //list.insert(list.end(), seats.begin(), seats.end());
        break;
    }
    return list;
}

//找出这一段时期的电脑存量，使用取交集的办法
Vector<std::string> RoomStorage::FindPeriodSeatsAvai(const std::string& timeStart, const std::string& timeLast) {
    Vector<std::string> list;
    Vector<std::string> times = generateTimeList(timeStart, timeLast);
    Vector<Vector<std::string>> vecs;
    for (size_t i = 0; i < times.size(); ++i) {
        Vector<std::string> seats = findAvaiComputer(times[i]);
        vecs.push_back(seats);
    }
    for (size_t i = 0; i < vecs.size(); ++i) {
        if (i == 0) {
            list = vecs[i];
        }
        else {
            Vector<std::string> temp;
            for(size_t j = 0; j < list.size(); ++j) {
                for(size_t k = 0; k < vecs[i].size(); ++k) {
                    if (list[j] == vecs[i][k]) {
                        temp.push_back(list[j]);
                        break;
                    }
                }
            }
            list = temp;
        }
    }
    return list;
}

Vector<Vector<std::string>> RoomStorage::GetTableMap() {
    Vector<Vector<std::string>> map;
    //std::string period = parseTimeToPeriod(time);
    std::string findAvaiComputerSQL = "SELECT * FROM computer_usage1";
    sqlite3pp::query qry(db, findAvaiComputerSQL.c_str());
    for (auto v : qry) {
        Vector<std::string> list;
        std::string computer_id, time_0, time_1, time_2, time_3, time_4, time_5, time_6, time_7, time_8, time_9, time_10, time_11, time_12, time_13, time_14, time_15, time_16, time_17, time_18, time_19, time_20, time_21, time_22, time_23;
        v.getter() >> computer_id >> time_0 >> time_1 >> time_2 >> time_3 >> time_4 >> time_5 >> time_6 >> time_7 >> time_8 >> time_9 >> time_10 >> time_11 >> time_12 >> time_13 >> time_14 >> time_15 >> time_16 >> time_17 >> time_18 >> time_19 >> time_20 >> time_21 >> time_22 >> time_23;
        //cout << "\t" << name << "\t" << phone << endl;
        list.push_back(computer_id);
        list.push_back(time_0);
        list.push_back(time_1);
        list.push_back(time_2);
        list.push_back(time_3);
        list.push_back(time_4);
        list.push_back(time_5);
        list.push_back(time_6);
        list.push_back(time_7);
        list.push_back(time_8);
        list.push_back(time_9);
        list.push_back(time_10);
        list.push_back(time_11);
        list.push_back(time_12);
        list.push_back(time_13);
        list.push_back(time_14);
        list.push_back(time_15);
        list.push_back(time_16);
        list.push_back(time_17);
        list.push_back(time_18);
        list.push_back(time_19);
        list.push_back(time_20);
        list.push_back(time_21);
        list.push_back(time_22);
        list.push_back(time_23);
        map.push_back(list);
    }
    return map;
}