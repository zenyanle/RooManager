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
    
    Vector<Seat> heap;      // 最大堆数组
    Vector<Seat*> seats;    // 直接访问数组，存储指向堆中元素的指针

    void siftDown(int index) {
        int size = heap.size();
        while (true) {
            int largest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < size && heap[left].usageCount > heap[largest].usageCount) {
                largest = left;
            }
            if (right < size && heap[right].usageCount > heap[largest].usageCount) {
                largest = right;
            }

            if (largest == index) {
                break;
            }

            Seat temp = heap[index];
            heap[index] = heap[largest];
            heap[largest] = temp;
            
            heap[index].index = index;
            heap[largest].index = largest;
            
            seats[heap[index].code - 1] = &heap[index];
            seats[heap[largest].code - 1] = &heap[largest];
            
            index = largest;
        }
    }

    void buildMaxHeap() {
        for (int i = heap.size() / 2 - 1; i >= 0; i--) {
            siftDown(i);
        }
    }

public:
    // 构造函数：初始化20个座位
    SeatHeap() {
        // 先预留空间
        for (int i = 0; i < 20; i++) {
            seats.push_back(nullptr);
        }
        
        // 初始化堆
        for (int i = 0; i < 20; i++) {
            heap.push_back(Seat(i + 1, 0));
            heap[i].index = i;
            seats[i] = &heap[i];
        }
        
        buildMaxHeap();
    }

    // 增加指定座位的使用次数
    bool increment(int seatCode) {
        if (seatCode < 1 || seatCode > 20) return false;
        
        Seat* seat = seats[seatCode - 1];
        seat->usageCount++;
        siftDown(seat->index);
        return true;
    }

    // 获取最受欢迎的座位
    Seat getTopSeat() const {
        if (heap.empty()) {
            return Seat();
        }
        return heap[0];
    }

    // 获取前N个最受欢迎的座位
    Vector<Seat> getTopN(int n) const {
        Vector<Seat> result;
        int count = std::min(n, (int)heap.size());
        for (int i = 0; i < count; i++) {
            result.push_back(heap[i]);
        }
        return result;
    }

    // 打印当前状态
    void print() const {
        std::cout << "【" << "2024-12-21 05:38:41" << "】" << std::endl;
        std::cout << "【" << "zenyanle" << "】" << std::endl;
        std::cout << "座位使用情况：" << std::endl;
        for (int i = 0; i < heap.size(); i++) {
            std::cout << "座位 " << heap[i].code 
                      << ": " << heap[i].usageCount << "次" << std::endl;
        }
    }
};

#endif // SEAT_HEAP_H