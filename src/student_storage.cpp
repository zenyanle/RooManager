#include "student_storage.h"
#include <iostream>
#include <cstring>
#include <iomanip> 
#include <sstream>


StudentStorage::StudentStorage(const std::string& db_name)
    : db(db_name.c_str()) {
        
    std::cout << "Database connected: " << db_name << std::endl;
}

StudentStorage::~StudentStorage() {
    std::cout << "Database closed" << std::endl;
}

void StudentStorage::creatTable() {
    const char* createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS Students(
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT NOT NULL,
            Phone TEXT NOT NULL,
            Passwd TEXT NOT NULL,
            Amout REAL NOT NULL
        );
    )";
    db.execute(createTableSQL);
}

void StudentStorage::RegisterStudent(std::string name, std::string phone, std::string passwd, std::string amout) {
    sqlite3pp::command cmd(
        db, "INSERT INTO Students (Name, Phone, Passwd, Amout) VALUES (?100, ?101, ?102, 20.0)");
    //const char* registerStudentSQL = "INSERT INTO Students2 (Name, Phone, Passwd, Amout) VALUES (?100, ?101, ?102, 20.0);"
    cmd.bind(100, name, sqlite3pp::nocopy);
    cmd.bind(101, phone, sqlite3pp::nocopy);
    cmd.bind(102, passwd, sqlite3pp::nocopy);
    cmd.execute();
}

void StudentStorage::RechargeAmout(float money, const std::string& phone_number) {
    //std::string rechargeSQL = "UPDATE Students SET Amout = Amout + " + std::to_string(money) + " WHERE Name = 'test';";
    //db.execute(rechargeSQL.c_str());
    sqlite3pp::command cmd(db, "UPDATE Students SET Amout = Amout + ?100 WHERE Phone = ?101");
    cmd.bind(100, std::to_string(money), sqlite3pp::nocopy);
    cmd.bind(101, phone_number, sqlite3pp::nocopy);
    //cmd.bind(102, phone_number, sqlite3pp::nocopy);
    cmd.execute();
}

void StudentStorage::CostAmout(float money, const std::string& phone_number) {
    //std::string costSQL = "UPDATE Students SET Amout = Amout - " + std::to_string(money) + " WHERE Name = 'test';";
    //db.execute(costSQL.c_str());
    sqlite3pp::command cmd(db, "UPDATE Students SET Amout = Amout - ?100 WHERE Phone = ?101");
    cmd.bind(100, std::to_string(money), sqlite3pp::nocopy);
    cmd.bind(101, phone_number, sqlite3pp::nocopy);
    //cmd.bind(102, phone_number, sqlite3pp::nocopy);
    cmd.execute();
}

Vector<std::string> StudentStorage::GetInfo(std::string Phone) {
    Vector<std::string> list;
    //const char* getAmoutSQL = "SELECT Name, Phone, Passwd, Amout FROM Students WHERE Phone = '111';";
    std::string getAmoutSQL = "SELECT Name, Phone, Passwd, Amout FROM Students WHERE Phone = '" + Phone + "';";
    sqlite3pp::query qry(db, getAmoutSQL.c_str());
    for (auto v : qry) {
        std::string name, phone, passwd, amout;
        v.getter() >> name >> phone >> passwd >> amout;


        list.push_back(name);
        list.push_back(phone);
        list.push_back(passwd);
        list.push_back(amout);
        //cout << "\t" << name << "\t" << phone << endl;
        //list.push_back(computer_id);
    }
    return list;
}

