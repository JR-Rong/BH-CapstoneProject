/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2023-03-09 19:35:30
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-03-09 19:37:45
 * @Description: file content
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <iterator>
#include <algorithm>

namespace utils {

std::vector<std::vector<double>> gen_dist_map(std::vector<double> map_arr);
void print_map(std::vector<std::vector<double>> map_);

class Logger {
private:
    std::ofstream m_log;

public:
    Logger();
    template <typename T>
    Logger& operator<<(T& value){
        std::cout << value << std::endl;
        this -> m_log << value << std::endl;
        return *this;
    }       
    void stroe_log();
};

} // namespace utils

#endif // UTILS_HPP_