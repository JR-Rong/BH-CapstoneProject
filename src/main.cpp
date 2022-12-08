/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-05 17:34:57
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2022-12-08 11:36:51
 * @Description: file content
 */
#include <iostream>

#include "de/differential_evolution.hpp"

int main(){
    de::double_constrain con(100, 1, 151);
    de::population pop(10, 15, con);
    std::cout << pop << std::endl;
    return 0;
}