/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-05 17:34:57
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2022-12-08 10:08:23
 * @Description: file content
 */
#include <iostream>

#include "de/differential_evolution.hpp"

int main(){
    de::double_constrain con(100, 1, 151);
    std::cout << con.get_max() << " " << con.get_rand_value() << std::endl;
    de::individual ind(10);
    ind.init(con);
    std::cout << ind << std::endl;
    return 0;
}