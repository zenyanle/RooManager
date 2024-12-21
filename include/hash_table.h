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