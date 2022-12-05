/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-05 17:34:57
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2022-12-05 21:29:02
 * @Description: file content
 */
#include <iostream>

#include "de/differential_evolution.hpp"

int main(){
    de::double_constrain con(100, 1, 151);
    std::cout << con.get_max() << " " << con.get_rand_value() << std::endl;
    return 0;
}