#ifndef CORE_H
#define CORE_H

#include "room_storage.h"
#include "student_storage.h"
#include "reserv_storage.h"
#include "crow_service.h"
#include "my_vector.h"
#include "linklist.h"
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>
#include <atomic>
#include <sparse_matrix.h>
#include <hash_table.h>
#include "seat_heap.h"

class Core {
    RoomStorage* roomStorage = nullptr;
    StudentStorage* studentStorage = nullptr;
    ReservStorage* reservStorage = nullptr;
    CrowService* crowService = nullptr;
    SeatHeap seats;

public:
    Core() {}

    void setRoomStorage(RoomStorage* roomStorageInjection);
    void setStudentStorage(StudentStorage* studentStorageInjection);
    void setReservStorage(ReservStorage* reservStorageInjection);
    void setCrowService(CrowService* crowServiceInjection);
    void resetRoomStorage();
    void ShowRoomMap();
    //void simulateStudentStorage();
    void StartWeb();
    void StartThreads();
    std::string DisplayStudentRank();


private:
    // 线程数据结构
    int getTimeDifferenceInHours(const std::string& inputTime);
    std::string getCurrentTimeFormatted();
    std::string getFullFormattedTime();
    bool isTimeToEnd(const std::string& endTimeStr);
    std::array<std::string, 2> getTimePeriod(int hour, int minute);
    Vector<std::string> getPeriodSeats(const std::string& time_start, const std::string& time_stop);
    void leaveComputer(std::string& phone_number, std::string& status);
    void deleteReserv(std::string& computer_id, std::string& phone_number, std::string& status);
    Vector<std::string> getAvaiSeatNow();
    std::string generateOccupancyJson();
    void setupRoutes();
    void CleanRecords();
    void AutoRegister();
    std::string sparseMatrixToJson(const SparseMatrix& matrix);
    void registerComputer(std::string& timeStart, std::string& timeLast, std::string& computer_id, std::string& status, std::string& phone_number);
    struct ThreadControl {
        std::thread* clearThread;
        std::thread* queueThread;
        std::atomic<bool>* running;
    }* m_control = nullptr;
};



#endif // CORE_H
