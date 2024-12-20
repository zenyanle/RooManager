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