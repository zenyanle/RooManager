cmake_minimum_required(VERSION 3.20)
project(RooManager)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 SQLite3 库
find_package(SQLite3 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/asio/asio/include)

# 添加头文件路径
include_directories(include ${SQLite3_INCLUDE_DIRS})

# 查找源文件
file(GLOB SOURCES "src/*.cpp")

# 生成可执行文件
add_executable(Rmanager ${SOURCES})

# 链接 SQLite3 和 OpenSSL 库
target_link_libraries(Rmanager ${SQLite3_LIBRARIES})
