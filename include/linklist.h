#ifndef LINKLIST_H
#define LINKLIST_H

#include <iostream>
#include <iomanip>

struct Event {
    int num;
    std::string start_time;
    std::string end_time;

    Event(int num_, const std::string& start_time_, const std::string& end_time_) : num(num_), start_time(start_time_), end_time(end_time_) {} // 构造函数
};

struct Node {
    const Event data;      // 节点存储的数据
    Node* next;    // 指向下一个节点的指针

    Node(const Event& e) : data(e), next(nullptr) {} // 构造函数
};

// 链表类
class LinkedList {
public:
    LinkedList() : head(nullptr), size(0) {} // 构造函数，初始化为空链表

    // 向链表末尾插入一个节点
    void insert(const Event& e) {
        Node* newNode = new Node(e);
        if (head == nullptr) {
            // 如果链表为空，新节点就是头节点
            head = newNode;
        } else {
            // 否则，找到链表的末尾，将新节点添加到末尾
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        ++size;
        //std::cout << "插入: " << val << std::endl;
    }

    // 打印链表中的所有节点
    void print() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->data.num << std::setw(4) << current->data.start_time << std::setw(4) << current->data.end_time << std::endl;
            current = current->next;
        }
        std::cout << "nullptr" << std::endl;
    }

    void remove(int number) {
        if (head == nullptr) {
            std::cout << "Err1";
            return;
        }
        if (head->data.num == number) {
            Node* temp = head;
            head = head->next;
            delete temp;
            --size;
            return;
        }
        Node* current = head;
        while (current->next != nullptr) {
            if (number == current->next->data.num) {
                Node* temp = current->next;
                current->next = current->next->next;
                delete temp;
                --size;
                return;
            }
            current = current->next;
        }
        return;
    }

    // 获取链表的长度
    int getSize() const {
        return size;
    }

    // 析构函数，释放所有节点
    ~LinkedList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        std::cout << "链表已销毁" << std::endl;
    }

private:
    Node* head; // 链表的头节点
    int size;   // 链表中节点的个数
};

#endif