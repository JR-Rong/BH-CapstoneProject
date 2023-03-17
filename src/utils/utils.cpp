/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2023-03-09 19:37:29
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-03-10 10:03:50
 * @Description: file content
 */
#include "utils/utils.hpp"

namespace utils {

std::vector<std::vector<double>> gen_dist_map(std::vector<double> map_arr) {
    if(map_arr.size() % 2 != 0) return {};

    std::vector<std::vector<double>> map_(map_arr.size() / 2);
    for(int i=0; i<map_arr.size() / 2; i++ ){
        for(int j=0; j<=i; j++){
            if(i == j) map_[i].push_back(0);
            else map_[i].push_back(sqrt(
                pow(map_arr[2*j]-map_arr[2*i], 2) + pow(map_arr[2*j+1]-map_arr[2*i+1], 2)
                ));
        }
    }

    return map_;
}

void print_map(std::vector<std::vector<double>> map_) {
    for(auto vec : map_){
        for(auto num : vec){
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}

} // namespace utils