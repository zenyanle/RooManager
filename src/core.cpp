#include "core.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

// 实现 setRoomStorage 方法
void Core::setRoomStorage(RoomStorage* roomStorageInjection) {
    // 检查传入的指针是否为 nullptr
    if (!roomStorageInjection) {
        throw std::invalid_argument("RoomStorage must not be null");
    }
    roomStorage = roomStorageInjection; // 设置依赖
}

void Core::setStudentStorage(StudentStorage* studentStorageinjection) {
    if (!studentStorageinjection) {
        throw std::invalid_argument("RoomStorage must not be null");
    }
    studentStorage = studentStorageinjection; // 设置依赖
}

void Core::setReservStorage(ReservStorage* reservStorageInjection) {
    if (!reservStorageInjection) {
        throw std::invalid_argument("RoomStorage must not be null");
    }
    reservStorage = reservStorageInjection; // 设置依赖
}

void Core::setCrowService(CrowService* crowServiceInjection) {
        crowService = crowServiceInjection;
        // 在这里设置所有的路由
        setupRoutes();
    }


void Core::resetRoomStorage() {
    roomStorage->ResetAll();
}

std::string Core::getCurrentTimeFormatted() {
    // 获取当前时间
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    // 格式化时间为 "8:30" 的形式
    std::ostringstream oss;
    oss << std::setw(1) << localTime->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << localTime->tm_min;

    return oss.str();
}

std::array<std::string, 2> Core::getTimePeriod(int hour, int minute) {
    int startMinute = (minute < 30) ? 30 : 0;
    int startHour = (minute < 30) ? hour : hour + 1;

    std::ostringstream start, end;
    start << hour << ":" << std::setw(2) << std::setfill('0') << startMinute;
    end << hour << ":" << std::setw(2) << std::setfill('0') << startHour;
    return {start.str(), end.str()};
}

int Core::getTimeDifferenceInHours(const std::string& inputTime) {
    // 解析传入的时间字符串
    int inputHour, inputMinute;
    char colon;
    std::istringstream iss(inputTime);
    iss >> inputHour >> colon >> inputMinute;

    // 获取当前时间
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    int currentHour = localTime->tm_hour;
    int currentMinute = localTime->tm_min;

    // 计算小时差
    int hourDifference = std::abs(currentHour - inputHour);

    // 如果分钟有差异，小时差+1
    if (currentMinute != inputMinute) {
        hourDifference++;
    }

    // 如果小时差为 0，补足为 1 小时
    if (hourDifference == 0) {
        hourDifference = 1;
    }

    return hourDifference;
}

bool Core::isTimeToEnd(const std::string& endTimeStr) {
    // 获取当前时间
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* currentTm = std::localtime(&currentTime);

    // 解析时间字符串
    int hour, minute;
    sscanf(endTimeStr.c_str(), "%d:%d", &hour, &minute);

    // 创建结束时间
    std::tm endTm = *currentTm;
    endTm.tm_hour = hour;
    endTm.tm_min = minute;

    // 将结束时间提前1分钟
    if (endTm.tm_min == 0) {
        endTm.tm_hour--;
        endTm.tm_min = 59;
    } else {
        endTm.tm_min--;
    }

    return (currentTm->tm_hour > endTm.tm_hour || 
           (currentTm->tm_hour == endTm.tm_hour && currentTm->tm_min >= endTm.tm_min));
}

std::string Core::getFullFormattedTime() {
    // 获取当前时间
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    // 格式化时间为 "YYYY:M:D:H:M" 的形式
    std::ostringstream oss;
    oss << (1900 + localTime->tm_year) << ":"  // 年
        << (1 + localTime->tm_mon) << ":"      // 月
        << localTime->tm_mday << ":"           // 日
        << localTime->tm_hour << ":"           // 时
        << localTime->tm_min;                  // 分

    return oss.str();
}

std::string convert2NextTime(const std::string& timeStr) {
    // 解析小时
    int hours = std::stoi(timeStr.substr(0, timeStr.find(':')));
    int minutes = std::stoi(timeStr.substr(timeStr.find(':') + 1));
    
    // 时间转换：如果分钟是0则加30，如果是30则加30并进位
    if (minutes == 0) {
        minutes = 30;
    } else {  // minutes == 30
        minutes = 0;
        hours++;
    }
    
    // 处理24小时进位
    if (hours >= 24) {
        hours = 0;
    }
    
    // 格式化输出
    return std::to_string(hours) + ":" + (minutes == 0 ? "00" : "30");
}

void Core::ShowRoomMap() {
    Vector<Vector<std::string>> map = roomStorage->GetTableMap();
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            std::cout << std::setw(4) << map[i][j]; // 使用 setw 设置列宽，保证对齐
        }
        std::cout << std::endl; // 输出每一行后换行
    }
}

//输入预约信息完成预定
void Core::registerComputer(std::string& timeStart, std::string& timeLast, std::string& computer_id, std::string& status, std::string& phone_number) {
    roomStorage->ChangePeriodStatus(timeStart, timeLast, computer_id, status);
    reservStorage->addRecord(0, phone_number, std::stoi(computer_id), 0, timeStart, timeLast, getFullFormattedTime());
    seats->increment(std::stoi(computer_id));
}

//查询某一时间段空闲机位：根据用户输入时间，输出该时间段内空闲机位信息
Vector<std::string> Core::getPeriodSeats(const std::string& time_start, const std::string& time_stop) {
    return roomStorage->FindPeriodSeatsAvai(time_start, time_stop);
}

void Core::leaveComputer(std::string& phone_number, std::string& status) {
    Vector<std::string> list = reservStorage->GetPeriod(phone_number);
    roomStorage->ChangePeriodStatus(list[0], list[1], list[2], status);
    float money = getTimeDifferenceInHours(list[0]);
    studentStorage->CostAmout(money, phone_number);
    reservStorage->finishRecord(phone_number, getFullFormattedTime());
}

//根据输入的时间，机器号撤销该时间的预定
void Core::deleteReserv(std::string& computer_id, std::string& phone_number, std::string& status) {
    Vector<std::string> list = reservStorage->GetPeriod(phone_number);
    roomStorage->ChangePeriodStatus(list[0], list[1], list[2], status);
    reservStorage->finishRecord(phone_number, getFullFormattedTime());
}

//找到最近的可用位置
Vector<std::string> Core::getAvaiSeatNow() {
    Vector<std::string> list = roomStorage->FindClosestSolution(getCurrentTimeFormatted());
    return list;
}

//排队人自动注册
void Core::AutoRegister() {
    //std::string currentTime = getCurrentTimeFormatted();
    std::string currentTime = "13:00";
    std::string statusStr = "1";
    Vector<std::string> list = roomStorage->findAvaiComputer(currentTime);
    Vector<std::string> nums = reservStorage->GetWaitingRecords();
    if (!list.empty() && !nums.empty()) {
        for (size_t i = 0; i < std::min(list.size(), nums.size()); i++) {
            reservStorage->finishRecord(nums[i], getFullFormattedTime());
            std::string nextTime = convert2NextTime(currentTime);
            registerComputer(currentTime, nextTime, list[i], statusStr, nums[i]);
        }
    }
}

std::string Core::generateOccupancyJson() {
    Vector<Vector<std::string>> occupancyData = roomStorage->GetTableMap();
    // 时间段数组
    std::vector<std::string> timeSlots = {
        "8:00-8:30", "8:30-9:00",
        "9:00-9:30", "9:30-10:00",
        "10:00-10:30", "10:30-11:00",
        "11:00-11:30", "11:30-12:00",
        "12:00-12:30", "12:30-13:00",
        "13:00-13:30", "13:30-14:00",
        "14:00-14:30", "14:30-15:00",
        "15:00-15:30", "15:30-16:00",
        "16:00-16:30", "16:30-17:00",
        "17:00-17:30", "17:30-18:00",
        "18:00-18:30", "18:30-19:00",
        "19:00-19:30", "19:30-20:00"
    };

    using json = nlohmann::json;

    json jsonData = json::array();

    // 检查 occupancyData 是否为空
    if (occupancyData.empty()) {
        // 返回空的 JSON 数组或适当的错误信息
        return jsonData.dump(4);
    }

    for (size_t i = 0; i < occupancyData.size(); ++i) {
        // 检查 occupancyData[i] 是否为空
        if (occupancyData[i].empty()) {
            continue; // 跳过空的子向量
        }

        json seatInfo;
        seatInfo["seatNumber"] = i + 1; // 机位编号，从1开始
        seatInfo["occupancy"] = json::array();

        size_t minSize = timeSlots.size();
        for (size_t j = 0; j < minSize; ++j) {
            json timeSlotInfo;
            timeSlotInfo["time"] = timeSlots[j];
            timeSlotInfo["status"] = occupancyData[i][j + 1]; // 1为占用，0为空闲

            seatInfo["occupancy"].push_back(timeSlotInfo);
        }

        jsonData.push_back(seatInfo);
    }

    // 返回 JSON 字符串
    return jsonData.dump(4); // 缩进4个空格，方便阅读
}

void Core::setupRoutes() {
        if (!crowService) return;
        
        auto& app = crowService->getApp();
        
        // 注册电脑的路由
        CROW_ROUTE(app, "/api/leave")
        .methods("GET"_method)
        ([this](const crow::request& req) {
            //auto computerId = req.url_params.get("computer_id");
            //auto status = req.url_params.get("status");
            auto phoneNumber = req.url_params.get("phone_number");
            //auto status = req.url_params.get("status");
            
            if (!phoneNumber) {
                return crow::response(400, "Missing parameters");
            }

            //std::string computerIdStr = computerId;
            std::string statusStr = "0";
            std::string phoneNumberStr = phoneNumber;
            //std::string statusStr = status;
            
            try {
                this->leaveComputer(phoneNumberStr, statusStr);
                return crow::response(200, "success");
            } catch (const std::exception& e) {
                return crow::response(500, e.what());
            }
        });

        CROW_ROUTE(app, "/api/get_map")
        .methods("GET"_method)
        ([this](const crow::request& req) {
            try {
                auto jsonData = this->generateOccupancyJson();  // 返回 crow::json::wvalue
                return crow::response(jsonData);  // 直接返回 JSON
            } catch (const std::exception& e) {
                crow::json::wvalue error_response({
                    {"status", "error"},
                    {"message", e.what()}
                });
                return crow::response(500, error_response);
            }
        });

        CROW_ROUTE(app, "/api/register")
        .methods("GET"_method)
        ([this](const crow::request& req) {
            auto timeStart = req.url_params.get("timeStart");
            auto timeLast = req.url_params.get("timeLast");
            auto computerId = req.url_params.get("computer_id");
            //auto status = req.url_params.get("status");
            auto phoneNumber = req.url_params.get("phone_number");
            
            if (!timeStart || !timeLast || !computerId || !phoneNumber) {
                return crow::response(400, "Missing parameters");
            }

            std::string timeStartStr = timeStart;
            std::string timeLastStr = timeLast;
            std::string computerIdStr = computerId;
            std::string statusStr = "1";
            std::string phoneNumberStr = phoneNumber;
            
            try {
                this->registerComputer(timeStartStr, timeLastStr, computerIdStr, statusStr, phoneNumberStr);
                return crow::response(200, "success");
            } catch (const std::exception& e) {
                return crow::response(500, e.what());
            }
        });

        CROW_ROUTE(app, "/api/get_record")
        .methods("GET"_method)
        ([this](const crow::request& req) {
            auto phoneNumber = req.url_params.get("phone_number");
            
            if (!phoneNumber) {
                return crow::response(400, "Missing parameters");
            }

            std::string phoneNumberStr = phoneNumber;
            
            try {
                Vector<std::string> recs  = reservStorage->GetPeriod(phoneNumber);
                using json = nlohmann::json;
                json record;
                record["end"] = recs[1];
                record["start"] = recs[0];
                record["cid"] = recs[2];
                return crow::response(record.dump(4));
            } catch (const std::exception& e) {
                return crow::response(400);
            }
        });

        CROW_ROUTE(app, "/api/get_info")
        .methods("GET"_method)
        ([this](const crow::request& req) {
            auto phoneNumber = req.url_params.get("phone_number");
            
            if (!phoneNumber) {
                return crow::response(400, "Missing parameters");
            }

            std::string phoneNumberStr = phoneNumber;
            
            try {
                Vector<std::string> info  = studentStorage->GetInfo(phoneNumberStr);
                using json = nlohmann::json;
                json data;
                data["amout"] = info[3];
                data["phone"] = info[1];
                data["name"] = info[0];
                return crow::response(data.dump(4));
            } catch (const std::exception& e) {
                return crow::response(400);
            }
        });

        CROW_ROUTE(app, "/api/recharge")
        .methods("GET"_method)
        ([this](const crow::request& req) {
            auto phoneNumber = req.url_params.get("phone_number");
            auto amount = req.url_params.get("amount");
            
            if (!amount || !phoneNumber) {
                return crow::response(400, "Missing parameters");
            }

            std::string phoneNumberStr = phoneNumber;
            float amountStr = std::stoi(amount);
            
            try {
                studentStorage->RechargeAmout(amountStr, phoneNumberStr);
                return crow::response(200);
            } catch (const std::exception& e) {
                return crow::response(400);
            }
        });

        CROW_ROUTE(app, "/api/login")
        .methods("GET"_method)
        ([this](const crow::request& req) {
            auto phoneNumber = req.url_params.get("phone_number");
            auto password = req.url_params.get("password");
            
            if (!phoneNumber || !password) {
                return crow::response(400, "Missing parameters");
            }

            std::string phoneNumberStr = phoneNumber;
            
            try {
                Vector<std::string> info  = studentStorage->GetInfo(phoneNumberStr);
                if (password == info[2]) return crow::response(200);
                else return crow::response(400);
            } catch (const std::exception& e) {
                return crow::response(400);
            }
        });

        CROW_ROUTE(app, "/api/art")
        .methods("GET"_method)
        ([this](const crow::request& req) {
            try {
                this->AutoRegister();  // 返回 crow::json::wvalue
                return crow::response(200);  // 直接返回 JSON
            } catch (const std::exception& e) {
                crow::json::wvalue error_response({
                    {"status", "error"},
                    {"message", e.what()}
                });
                return crow::response(500, error_response);
            }
        });

        CROW_ROUTE(app, "/api/add_wait")
        .methods("GET"_method)
        ([this](const crow::request& req) {
            auto phoneNumber = req.url_params.get("phone_number");
            
            if (!phoneNumber) {
                return crow::response(400, "Missing parameters");
            }

            std::string phoneNumberStr = phoneNumber;
            
            try {
                //Vector<std::string> recs  = this->registerComputer(phoneNumber);
                reservStorage->addRecord(1, phoneNumber, 0, 0, "null", "null", getFullFormattedTime());
                return crow::response(200);
            } catch (const std::exception& e) {
                return crow::response(400);
            }
        });

        CROW_ROUTE(app, "/api/get_map1")
        .methods("GET"_method)
        ([this](const crow::request& req) {
            try {

                auto jsonData = sparseMatrixToJson(SparseMatrix(roomStorage->GetTableMap()));  // 返回 crow::json::wvalue
                return crow::response(jsonData);  // 直接返回 JSON
            } catch (const std::exception& e) {
                crow::json::wvalue error_response({
                    {"status", "error"},
                    {"message", e.what()}
                });
                return crow::response(500, error_response);
            }
        });

        CROW_ROUTE(app, "/api/get_ranks")
        .methods("GET"_method)
        ([this](const crow::request& req) {
            try {

                auto jsonData = DisplayStudentRank(); // 返回 crow::json::wvalue
                return crow::response(jsonData);  // 直接返回 JSON
            } catch (const std::exception& e) {
                crow::json::wvalue error_response({
                    {"status", "error"},
                    {"message", e.what()}
                });
                return crow::response(500, error_response);
            }
        });

        CROW_ROUTE(app, "/api/get_pop_seat")
        .methods("GET"_method)
        ([this](const crow::request& req) {
            try {

                auto jsonData = std::to_string(seats->getTopSeatCode()); // 返回 crow::json::wvalue
                return crow::response(jsonData);  // 直接返回 JSON
            } catch (const std::exception& e) {
                crow::json::wvalue error_response({
                    {"status", "error"},
                    {"message", e.what()}
                });
                return crow::response(500, error_response);
            }
        });

}

void Core::CleanRecords() {
    Vector<Vector<std::string>>records = reservStorage->GetActiveRecord();
    for (size_t i = 0; i < records.size(); i++) {
        if(isTimeToEnd(records[i][1])) {
            std::string statusStr = "0";
            leaveComputer(records[i][0], statusStr);
        }
    }
}

void Core::StartThreads() {
        auto running = new std::atomic<bool>(true);

        // 清退线程函数
        auto clearFunc = [](std::atomic<bool>* running, Core* instance) {
            using namespace std::chrono;
            while (*running) {
                auto now = system_clock::now();
                auto timeNow = system_clock::to_time_t(now);
                std::tm* tm_utc = std::gmtime(&timeNow);

                if ((tm_utc->tm_min == 29 || tm_utc->tm_min == 59) && tm_utc->tm_sec == 0) {
                    instance->CleanRecords();
                }

                std::this_thread::sleep_for(seconds(1));
            }
        };

        // 排队线程函数
        auto queueFunc = [](std::atomic<bool>* running, Core* instance) {
            using namespace std::chrono;
            while (*running) {
                instance->AutoRegister();
                std::this_thread::sleep_for(seconds(2));
            }
        };

        // 创建线程
        //new std::thread(clearFunc, running, this);
        new std::thread(queueFunc, running, this);
    }

    
void Core::StartWeb() {
    crowService->start(8989);
}

std::string Core::sparseMatrixToJson(const SparseMatrix& matrix) {
    using json = nlohmann::json;
    json j;
    j["rows"] = matrix.getRows();
    j["cols"] = matrix.getCols();
    j["count"] = matrix.getCount();
    
    json elements = json::array();
    Vector<Vector<std::string>> data = matrix.toVector();
    
    // 遍历矩阵，存储非零元素
    for (int i = 0; i < matrix.getRows(); i++) {
        for (int j = 0; j < matrix.getCols(); j++) {
            if (matrix.get(i, j) != "0") {
                elements.push_back({
                    {"row", i},
                    {"col", j},
                    {"value", matrix.get(i, j)}
                });
            }
        }
    }
    j["elements"] = elements;
    
    return j.dump(4); 
}

std::string Core::DisplayStudentRank() {
    HashTable phoneBook(100);
    reservStorage->SumStudents(phoneBook);
    return phoneBook.toJson();
}