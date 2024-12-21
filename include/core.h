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
    //基础服务定义
    RoomStorage* roomStorage = nullptr;
    StudentStorage* studentStorage = nullptr;
    ReservStorage* reservStorage = nullptr;
    CrowService* crowService = nullptr;
    SeatHeap* seats;

public:
    Core() {
        //初始化堆排序
        seats = new SeatHeap();
    }
    
    //依赖注入
    void setRoomStorage(RoomStorage* roomStorageInjection);
    void setStudentStorage(StudentStorage* studentStorageInjection);
    void setReservStorage(ReservStorage* reservStorageInjection);
    void setCrowService(CrowService* crowServiceInjection);

    //重置座位占用
    void resetRoomStorage();

    //在终端输出座位占用
    void ShowRoomMap();
    //void simulateStudentStorage();

    //开启web服务
    void StartWeb();

    //开启服务线程
    void StartThreads();

    //获取学生排名
    std::string DisplayStudentRank();


private:
    //一些时间转换和运算的方法
    int getTimeDifferenceInHours(const std::string& inputTime);
    std::string getCurrentTimeFormatted();
    std::string getFullFormattedTime();
    bool isTimeToEnd(const std::string& endTimeStr);
    std::array<std::string, 2> getTimePeriod(int hour, int minute);
    Vector<std::string> getPeriodSeats(const std::string& time_start, const std::string& time_stop);


    //下机操作
    void leaveComputer(std::string& phone_number, std::string& status);

    //删除预约记录
    void deleteReserv(std::string& computer_id, std::string& phone_number, std::string& status);

    //获取当前可用的座位信息
    Vector<std::string> getAvaiSeatNow();

    //生成位置占用情况的json
    std::string generateOccupancyJson();

    //设置网络服务器路由
    void setupRoutes();

    //清理过期座位数据
    void CleanRecords();

    //自动预约座位
    void AutoRegister();

    //将位置占用的稀疏矩阵转为json
    std::string sparseMatrixToJson(const SparseMatrix& matrix);

    //预约某个时段的计算机座位
    void registerComputer(std::string& timeStart, std::string& timeLast, std::string& computer_id, std::string& status, std::string& phone_number);

    //线程数据结构，用于轮询
    struct ThreadControl {
        std::thread* clearThread;
        std::thread* queueThread;
        std::atomic<bool>* running;
    }* m_control = nullptr;
};



#endif // CORE_H
