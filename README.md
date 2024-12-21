### 变长数组

```cpp
// my_vector.h
#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <iostream>
#include <stdexcept>

template <typename T>
class Vector {
public:
    // 默认构造函数，初始容量设置为4
    Vector() : size_(0), capacity_(4), data_(new T[capacity_]) {}

    // 拷贝构造函数
    Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_), data_(new T[other.capacity_]) {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    // 拷贝赋值运算符
    Vector& operator=(const Vector& other) {
        if (this == &other) return *this;

        delete[] data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
        return *this;
    }

    // 析构函数，释放内存
    ~Vector() {
        delete[] data_;
    }

    // 获取当前元素个数
    size_t size() const {
        return size_;
    }

    // 获取当前容量
    size_t capacity() const {
        return capacity_;
    }

    // 判断是否为空
    bool empty() const {
        return size_ == 0;
    }

    // 访问指定位置的元素
    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    // 向末尾添加元素
    void push_back(const T& value) {
        if (size_ == capacity_) {
            // 如果容量不够，则扩容
            resize(capacity_ * 2);
        }
        data_[size_] = value;
        ++size_;
    }

    // 移除末尾元素
    void pop_back() {
        if (size_ > 0) {
            --size_;
        } else {
            throw std::out_of_range("Pop from empty vector");
        }
    }

    // 获取末尾元素
    T& back() {
        if (size_ > 0) {
            return data_[size_ - 1];
        } else {
            throw std::out_of_range("No elements in vector");
        }
    }

private:
    // 动态调整数组容量
    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        // 拷贝旧数据到新数组
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        // 释放旧数据
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    size_t size_;      // 当前元素个数
    size_t capacity_;  // 当前容量
    T* data_;          // 指向动态数组的指针
};

#endif // MY_VECTOR_H

```

这是一个模板类 Vector 的实现，是一个自定义的动态数组容器。它使用一个指向动态分配内存的指针 data_ 来存储数据，通过 size_ 记录当前元素个数，capacity_ 记录当前容量。当容量不足时会自动扩容（容量翻倍）。它实现了基本的增删查改操作（push_back、pop_back、operator[]等），并通过拷贝构造函数和赋值运算符确保了深拷贝，避免内存共享问题。整体功能类似于 C++ 标准库中的 std::vector。

### 哈希表

```cpp
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <iostream>
#include "my_vector.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class HashTable {
private:
    // 定义哈希表项的结构
    struct HashItem {
        std::string phoneNumber;  // 电话号码
        int value;               // 映射的值
        bool isOccupied;        // 该位置是否被占用
        bool isDeleted;         // 该位置是否被删除

        HashItem() : isOccupied(false), isDeleted(false) {}
    };

    HashItem* table;    // 哈希表数组
    int capacity;       // 哈希表容量
    int size;          // 当前存储的元素个数

    // 哈希函数，将电话号码转换为索引
    int hashFunction(const std::string& phoneNumber) {
        int hash = 0;
        for (char c : phoneNumber) {
            hash = (hash * 31 + (c - '0')) % capacity;
        }
        return hash;
    }

public:
    // 构造函数
    explicit HashTable(int tableSize = 100) : capacity(tableSize), size(0) {
        table = new HashItem[capacity];
    }

    // 析构函数
    ~HashTable() {
        delete[] table;
    }

    // 插入方法
    bool insert(const std::string& phoneNumber, int value) {
        if (size >= capacity) {
            return false;  // 哈希表已满
        }

        int index = hashFunction(phoneNumber);
        int originalIndex = index;

        // 线性探测
        while (table[index].isOccupied && !table[index].isDeleted) {
            if (table[index].phoneNumber == phoneNumber) {
                return false;  // 电话号码已存在
            }
            index = (index + 1) % capacity;  // 线性探测下一个位置
            if (index == originalIndex) {
                return false;  // 已经遍历了整个表
            }
        }

        // 找到可用位置，插入数据
        table[index].phoneNumber = phoneNumber;
        table[index].value = value;
        table[index].isOccupied = true;
        table[index].isDeleted = false;
        size++;
        std::cout << index << std::endl;
        return true;
    }

    // 查找方法
    bool search(const std::string& phoneNumber, int& value) {
        int index = hashFunction(phoneNumber);
        int originalIndex = index;

        while (table[index].isOccupied || table[index].isDeleted) {
            if (table[index].isOccupied && !table[index].isDeleted && 
                table[index].phoneNumber == phoneNumber) {
                value = table[index].value;
                return true;
            }
            index = (index + 1) % capacity;
            if (index == originalIndex) {
                break;
            }
        }
        return false;
    }

    // 删除方法
    bool remove(const std::string& phoneNumber) {
        int index = hashFunction(phoneNumber);
        int originalIndex = index;

        while (table[index].isOccupied || table[index].isDeleted) {
            if (table[index].isOccupied && !table[index].isDeleted && 
                table[index].phoneNumber == phoneNumber) {
                table[index].isDeleted = true;
                size--;
                return true;
            }
            index = (index + 1) % capacity;
            if (index == originalIndex) {
                break;
            }
        }
        return false;
    }

    // 增加值方法
    bool increment(const std::string& phoneNumber) {
        int index = hashFunction(phoneNumber);
        int originalIndex = index;

        while (table[index].isOccupied || table[index].isDeleted) {
            if (table[index].isOccupied && !table[index].isDeleted && 
                table[index].phoneNumber == phoneNumber) {
                table[index].value++;
                return true;
            }
            index = (index + 1) % capacity;
            if (index == originalIndex) {
                break;
            }
        }
        return false;
    }

    Vector<std::pair<std::string, int>> bubbleSortByValue() {
        Vector<std::pair<std::string, int>> items;
        
        // 收集有效元素
        for (int i = 0; i < capacity; i++) {
            if (table[i].isOccupied && !table[i].isDeleted) {
                items.push_back({table[i].phoneNumber, table[i].value});
            }
        }

        // 冒泡排序（从大到小）
        int n = items.size();
        for (size_t i = 0; i < n - 1; i++) {
            for (size_t j = 0; j < n - i - 1; j++) {
                if (items[j].second < items[j + 1].second) {
                    std::swap(items[j], items[j + 1]);
                }
            }
        }

        return items;
    }

    // 打印排序结果方法
    void printSortedByValue() {
        Vector<std::pair<std::string, int>> sortedItems = bubbleSortByValue();
        std::cout << "Sorted by value (ascending):\n";
        for (int i = 0; i < sortedItems.size(); i++) {
            std::cout << "Phone: " << sortedItems[i].first 
                     << ", Value: " << sortedItems[i].second << std::endl;
        }
    }

    // 获取当前元素个数
    int getSize() const {
        return size;
    }

    // 导出为JSON方法
    std::string toJson() {
        json j;
        j["capacity"] = capacity;
        j["size"] = size;
        
        // 先获取排序后的数据
        Vector<std::pair<std::string, int>> sortedItems = bubbleSortByValue();
        
        // 转换为JSON数组
        json items = json::array();
        for (size_t j = 0; j < sortedItems.size(); j++) {
            // 找到该电话号码在哈希表中的索引
            int index = -1;
            for (int i = 0; i < capacity; i++) {
                if (table[i].isOccupied && !table[i].isDeleted && 
                    table[i].phoneNumber == sortedItems[j].first) {
                    index = i;
                    break;
                }
            }
            
            items.push_back({
                {"phoneNumber", sortedItems[j].first},
                {"value", sortedItems[j].second},
                {"index", index}
            });
        }
        j["items"] = items;
        
        return j.dump(4);  // 使用4空格缩进
    }

};

#endif // HASH_TABLE_H
```

这是一个使用线性探测法解决冲突的哈希表实现，专门用于存储电话号码和对应的计数值。主要特点：使用 HashItem 结构存储数据（包含电话号码、值、占用标志和删除标志），通过字符串哈希函数将电话号码映射到索引位置，支持插入、查找、删除、增加计数值等基本操作。还实现了按值大小排序（使用冒泡排序）和 JSON 格式导出功能。整体实现了一个电话号码计数系统的基本框架。

### 稀疏矩阵

```cpp
#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "my_vector.h"

// 定义稀疏矩阵的数据结构
struct SparseElement {
    int row;     // 行号
    int col;     // 列号
    std::string value;  // 值

    SparseElement(int r, int c, const std::string& v) 
        : row(r), col(c), value(v) {}
};

class SparseMatrix {
private:
    std::vector<SparseElement> elements;  // 存储非零元素
    int rows;    // 矩阵总行数
    int cols;    // 矩阵总列数
    int count;   // 非零元素个数

public:
    // 构造函数
    SparseMatrix(const Vector<Vector<std::string>>& map) {
        rows = map.size();
        cols = map[0].size();  // 假设所有行的长度相同
        count = 0;

        // 遍历原始矩阵，只存储"1"
        for (int i = 0; i < rows; i++) {
            for (int j = 1; j < cols; j++) {
                if (map[i][j] == "1") {
                    elements.emplace_back(i, j, "1");
                    count++;
                }
            }
        }
    }

    // 获取指定位置的值
    std::string get(int row, int col) const {
        for (const auto& elem : elements) {
            if (elem.row == row && elem.col == col) {
                return elem.value;
            }
        }
        return "0";  // 如果没找到，返回"0"
    }

    // 设置指定位置的值
    void set(int row, int col, const std::string& value) {
        // 检查是否已存在
        for (auto& elem : elements) {
            if (elem.row == row && elem.col == col) {
                if (value == "0") {
                    // 如果新值是0，删除该元素
                    elements.erase(
                        std::remove_if(elements.begin(), elements.end(),
                            [row, col](const SparseElement& e) {
                                return e.row == row && e.col == col;
                            }),
                        elements.end()
                    );
                    count--;
                } else {
                    elem.value = value;
                }
                return;
            }
        }

        // 如果是新的非零元素，添加它
        if (value != "0") {
            elements.emplace_back(row, col, value);
            count++;
        }
    }

    // 打印稀疏矩阵
    void print() const {
        std::cout << "总行数: " << rows << std::endl;
        std::cout << "总列数: " << cols << std::endl;
        std::cout << "非零元素个数: " << count << std::endl;
        std::cout << "行\t列\t值" << std::endl;
        for (const auto& elem : elements) {
            std::cout << elem.row << "\t" 
                      << elem.col << "\t" 
                      << elem.value << std::endl;
        }
    }

    // 转回原始的二维Vector格式
    Vector<Vector<std::string>> toVector() const {
        Vector<Vector<std::string>> result;
        // 初始化每一行
        for (int i = 0; i < rows; i++) {
            Vector<std::string> row;
            // 初始化每一列为"0"
            for (int j = 0; j < cols; j++) {
                row.push_back("0");
            }
            result.push_back(row);
        }
        
        // 填充非零元素
        for (const auto& elem : elements) {
            result[elem.row][elem.col] = elem.value;
        }
        return result;
    }

    // 获取矩阵的基本信息
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    int getCount() const { return count; }
};

// 工具函数实现
inline SparseMatrix convertToSparseMatrix(const Vector<Vector<std::string>>& map) {
    return SparseMatrix(map);
}

#endif // SPARSE_MATRIX_H
```

这是一个稀疏矩阵（Sparse Matrix）的实现，专门用于高效存储具有大量"0"值的二维矩阵。它只存储非零元素（此处特指值为"1"的元素），使用 `SparseElement` 结构存储每个非零元素的行号、列号和值。主要功能包括矩阵构造、元素的获取和设置、矩阵打印，以及在稀疏格式和普通二维向量格式之间的转换。这种实现方式特别适合处理大型但大部分元素为零的矩阵，可以显著节省存储空间。

### 堆排序

```cpp
// seat_heap.h
#ifndef SEAT_HEAP_H
#define SEAT_HEAP_H

#include <iostream>
#include "my_vector.h"

class SeatHeap {
private:
    struct Seat {
        int code;        // 座位编号 1-20
        int usageCount;  // 使用次数
        int index;       // 在堆中的位置
        
        Seat(int c = 0, int u = 0) : code(c), usageCount(u), index(0) {}
    };
    
    Vector<Seat> heap;

    void siftUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent].usageCount < heap[index].usageCount) {
                // 交换节点
                Seat temp = heap[parent];
                heap[parent] = heap[index];
                heap[index] = temp;
                
                // 更新索引
                heap[parent].index = parent;
                heap[index].index = index;
                
                index = parent;
            } else {
                break;
            }
        }
    }

public:
    // 构造函数
    SeatHeap() {
        // 初始化堆
        for (int i = 0; i < 20; i++) {
            heap.push_back(Seat(i + 1, 0));
            heap[i].index = i;
        }
    }

    // 增加使用次数
    bool increment(int seatCode) {
        if (seatCode < 1 || seatCode > 20) {
            return false;
        }
        
        // 找到对应座位的索引
        for (int i = 0; i < heap.size(); i++) {
            if (heap[i].code == seatCode) {
                heap[i].usageCount++;
                siftUp(i);
                break;
            }
        }
        return true;
    }

    // 获取最热门座位的编号
    int getTopSeatCode() const {
        if (heap.empty()) {
            return 0;
        }
        return heap[0].code;
    }

    // 获取最热门座位的使用次数
    int getTopSeatUsage() const {
        if (heap.empty()) {
            return 0;
        }
        return heap[0].usageCount;
    }

    // 用于调试的打印函数
    void print() const {
        std::cout << "座位使用情况：" << std::endl;
        for (int i = 0; i < heap.size(); i++) {
            std::cout << "座位 " << heap[i].code 
                      << ": " << heap[i].usageCount << "次" 
                      << " (index: " << heap[i].index << ")" << std::endl;
        }
    }
};

#endif // SEAT_HEAP_H
```

这是一个使用最大堆实现的座位使用频率统计系统。它维护了20个座位的使用次数，并始终将使用次数最多的座位保持在堆顶。每个座位包含三个属性：座位编号(code)、使用次数(usageCount)和在堆中的位置(index)。核心操作是 siftUp 上浮操作，当某个座位的使用次数增加时，通过上浮操作维护最大堆的性质，确保使用次数最多的座位总是位于堆顶。主要功能包括增加座位使用次数(increment)和获取最热门座位信息(getTopSeatCode, getTopSeatUsage)。
