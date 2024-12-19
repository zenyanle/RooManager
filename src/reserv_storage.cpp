#include "reserv_storage.h"
#include <iostream>

ReservStorage::ReservStorage(const std::string& db_name)
    : db(db_name.c_str()) {
        
    std::cout << "Database connected: " << db_name << std::endl;
}

ReservStorage::~ReservStorage() {
    std::cout << "Database closed" << std::endl;
}

void ReservStorage::creatTable() {
    std::string createTableSQL = R"(
    CREATE TABLE IF NOT EXISTS Reservation (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        status INTEGER NOT NULL, 
        phone_number TEXT NOT NULL,
        computer_id INTEGER NOT NULL,
        is_completed INTEGER NOT NULL,
        start_time TEXT,
        end_time TEXT,
        reservation_start_time TEXT,
        reservation_end_time TEXT
    );
    )";
    db.execute(createTableSQL.c_str());
}

void ReservStorage::addRecord(int status, const std::string& phone_number, int computer_id, int is_completed, const std::string& start_time, const std::string& end_time, const std::string& fullCurrentTime) {
    sqlite3pp::command cmd(
        db, "INSERT INTO Reservation (status, phone_number, computer_id, is_completed, start_time, end_time, reservation_start_time, reservation_end_time) VALUES (?100, ?101, ?102, ?103, ?104, ?105, ?106, ?107)");

    // 绑定参数，确保绑定的顺序与 SQL 语句中的顺序一致
    cmd.bind(100, status);  // 直接绑定整数
    cmd.bind(101, phone_number, sqlite3pp::nocopy);
    cmd.bind(102, computer_id);  // 直接绑定整数
    cmd.bind(103, is_completed); // 直接绑定整数
    cmd.bind(104, start_time, sqlite3pp::nocopy);
    cmd.bind(105, end_time, sqlite3pp::nocopy);
    cmd.bind(106, fullCurrentTime, sqlite3pp::nocopy);
    cmd.bind(107, phone_number, sqlite3pp::nocopy);
    
    cmd.execute();
}



void ReservStorage::finishRecord(const std::string& phone_number, const std::string& fullCurrentTime) {
    sqlite3pp::command cmd(db, "UPDATE Reservation SET is_completed = ?100, reservation_end_time = ?101 WHERE phone_number = ?102 AND is_completed = 0");
    cmd.bind(100, std::to_string(1), sqlite3pp::nocopy);
    cmd.bind(101, fullCurrentTime, sqlite3pp::nocopy);
    cmd.bind(102, phone_number, sqlite3pp::nocopy);
    cmd.execute();
}

Vector<std::string> ReservStorage::GetPeriod(const std::string& phone_number) {
    Vector<std::string> list;
    std::string getPeriodSQL = "SELECT start_time, end_time, computer_id FROM Reservation WHERE phone_number = '" + phone_number + "' AND is_completed = 0;";
    sqlite3pp::query qry(db, getPeriodSQL.c_str());
    for (auto v : qry) {
        std::string start_time, end_time, computer_id;
        v.getter() >> start_time >> end_time >> computer_id;
        list.push_back(start_time);
        list.push_back(end_time);
        list.push_back(computer_id);
    }
    return list;
}

Vector<Vector<std::string>> ReservStorage::GetActiveRecord() {
    Vector<Vector<std::string>> list;
    std::string getActiveRecordSQL = "SELECT phone_number, end_time FROM Reservation WHERE status = 0 AND is_completed = 0;";
    sqlite3pp::query qry(db, getActiveRecordSQL.c_str());
    for (auto v : qry) {
        Vector<std::string> l;
        std::string phone_number, end_time;
        v.getter() >> phone_number >> end_time;
        l.push_back(phone_number);
        l.push_back(end_time);
        list.push_back(l);
    }
    return list;
}


void ReservStorage::AddStatusOneRecord(const std::string& phone_number) {
    sqlite3pp::command cmd(
        db, "INSERT INTO Reservation (status, phone_number, computer_id, is_completed, start_time, end_time, reservation_start_time, reservation_end_time) VALUES (1, ?100, NULL, 0, NULL, NULL, NULL, NULL)");
    
    // 绑定参数
    cmd.bind(100, phone_number, sqlite3pp::nocopy);
    cmd.execute();
}

// 查询 status 为 1 的所有记录
Vector<std::string> ReservStorage::GetWaitingRecords() {
    Vector<std::string> list;
    std::string getWaitingRecordsSQL = "SELECT phone_number FROM Reservation WHERE status = 1 AND is_completed = 0;";
    
    sqlite3pp::query qry(db, getWaitingRecordsSQL.c_str());
    for (auto v : qry) {
        std::string phone_number;
        v.getter() >> phone_number;
        list.push_back(phone_number);
    }
    return list;
}

