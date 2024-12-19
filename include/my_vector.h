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
