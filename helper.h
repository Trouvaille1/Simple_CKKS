//
// Created by hanggu on 24-4-17.
//

#ifndef SIMPLE_CKKS_HELPER_H
#define SIMPLE_CKKS_HELPER_H

#include <iomanip>
#include <iostream>
#include <complex>
#include <vector>

template<typename T>
void print_vector(const std::vector<T>& vec);


//必须要inline，否则会出现多重定义错误（因为没写helper.cpp）
template<typename T>
inline void print_vector(const std::vector<T>& vec)
{
    std::cout << "[";
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        std::cout << *it;
        if (std::next(it) != vec.end())
        {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

//Helper function: Print line number.
inline void print_line(int line_number)
{
    std::cout << "Line " << std::setw(3) << line_number << " --> ";
}

#endif //SIMPLE_CKKS_HELPER_H
