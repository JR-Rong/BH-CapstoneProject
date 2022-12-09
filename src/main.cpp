/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-05 17:34:57
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2022-12-09 11:58:03
 * @Description: file content
 */
#include <iostream>

#include "de/differential_evolution.hpp"

namespace de {
    #define VAR_COUNT 10
    #define POP_SIZE 12
} //namespace de

int main(){
    de::double_constrain con(100, 1, 151);
    de::population pop(POP_SIZE, VAR_COUNT, con);
    de::mutation_strategy mut(0.8, 0.2, VAR_COUNT);
    std::cout << pop << mut.get_dither() << std::endl;
    return 0;
}