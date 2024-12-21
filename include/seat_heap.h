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