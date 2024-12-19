// crow_service.h
#ifndef CROW_SERVICE_H
#define CROW_SERVICE_H

#include <crow.h>
#include <string>
#include <thread>

class CrowService {
private:
    crow::SimpleApp app;
    std::thread* server_thread;
    bool running;
    
public:
    CrowService() : server_thread(nullptr), running(false) {}

    void start(uint16_t port = 3000) {
        if (!running) {
            // 设置端口
            app.port(port);
            
            // 在新线程中启动服务器
            server_thread = new std::thread([this] {
                try {
                    this->app.run();
                } catch (const std::exception& e) {
                    std::cerr << "Server error: " << e.what() << std::endl;
                }
            });

            running = true;
            std::cout << "Server started on port " << port << std::endl;
        }
    }

    void stop() {
        if (running) {
            app.stop();
            if (server_thread && server_thread->joinable()) {
                server_thread->join();
                delete server_thread;
                server_thread = nullptr;
            }
            running = false;
            std::cout << "Server stopped" << std::endl;
        }
    }

    crow::SimpleApp& getApp() { return app; }

    ~CrowService() {
        stop();
    }
};

#endif