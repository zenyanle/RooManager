#include <iostream>
#include "core.h"

int main() {
    // 创建 RoomStorage 对象
    RoomStorage roomStorage("test.db"); // 假设构造函数需要数据库名
    StudentStorage studentStorage("test.db");
    ReservStorage reservStorage("test.db");
    CrowService crowService;


    // 创建 Core 对象
    Core core;

    try {
        // 设置 RoomStorage 依赖
        core.setRoomStorage(&roomStorage);
        core.setStudentStorage(&studentStorage);
        core.setReservStorage(&reservStorage);
        core.setCrowService(&crowService);


        core.resetRoomStorage();
        core.StartThreads();
        // 定义时间、教室 ID 和状态
        std::string timeStart = "8:00"; // 假设时间格式为 HH:MM
        std::string timeLast = "10:00"; 
        //std::string classroom_id = "12"; // 假设教室 ID
        std::string status = "1"; // 假设状态

        // 注册计算机        
        //core.registerComputer(timeStart, timeLast, classroom_id, status);
        core.ShowRoomMap();



    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    //core.simulateStudentStorage();
    core.StartWeb();

    return 0;
}