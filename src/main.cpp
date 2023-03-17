/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-05 17:34:57
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-03-10 15:20:12
 * @Description: file content
 */
#include <iostream>
#include <thread>
#include <functional>

#include "de/differential_evolution.hpp"
#include "mapping/mapping.hpp"
#include "utils/utils.hpp"

#include "test/att48.inl"

#define DE_VAR_COUNT 48
#define DE_IND_MAX  5
#define DE_POP_SIZE 20
#define DE_GEN_SIZE 50

std::vector<std::vector<double>> city_map = {
    {0, 1.414, 2.236, 3.605,2.828},
    {1.414, 0, 1, 2.236, 1.414},
    {2.236, 1, 0, 1.414, 1},
    {3.605, 2.236, 1.414, 0, 1,},
    {2.282, 1.414, 1, 1, 0},
};
double cost_fun(std::shared_ptr<de::individual>& ind_ptr){
    double res = 0;
    auto var = ind_ptr -> get_vars();

    std::vector<std::vector<std::pair<double, int>>> res_map(DE_IND_MAX);
    int idx = 0;
    for(auto num : var){
        int car_idx = static_cast<int>(num);
        res_map[car_idx].push_back(std::make_pair(num, idx++));
    }
    for(std::vector<std::pair<double, int>>& vec : res_map) {
        sort(vec.begin(), vec.end(), [](std::pair<double, int> x, std::pair<double, int> y){return x.first < y.first;} );
        std::pair<double, int> last_pair = {0, -1};
        for(std::pair<double, int> pair : vec){
            res += last_pair.second >= pair.second ? city_map[last_pair.second + 1][pair.second + 1] : city_map[pair.second + 1][last_pair.second + 1];
            last_pair = pair;
        }
    }

    return res;
}

int main(){

    // mapping::display_map map;
    
    // if(mapping::choose_build_map_way()){
    //     map.input_size();
    //     map.init();
    //     map.gen_map();
    //     map.display();
    //     mapping::store_map(map);
    // } else {
    //     #ifdef PROJECT_CURRENT_DIR
    //         std::string map_file = std::string(PROJECT_CURRENT_DIR) + "map_build\\map_default.json";
    //         mapping::read_map(map_file, map);
    //     #endif
    //     map.init();
    //     map.display(); 
    // }
    auto map_ = utils::gen_dist_map(city_pos);
    utils::print_map(map_);
    city_map = map_;

    de::constrains<de::double_constrain> cons(DE_IND_MAX, 0, (unsigned)time( NULL ), DE_POP_SIZE);
    de::population pop1(DE_POP_SIZE, DE_VAR_COUNT, cons);
    de::population pop2(DE_POP_SIZE, DE_VAR_COUNT, cons);
    de::mutation_strategy mut(0.8, 0.7, DE_VAR_COUNT);
    std::function<double(std::shared_ptr<de::individual>&)> cost_func{cost_fun};
    std::cout << pop1 << std::endl;
    std::shared_ptr<de::individual> bestInd = std::make_shared<de::individual>(DE_VAR_COUNT);
    de::termination_strategy terminationStrategy(de::termination_strategy_max_gen{DE_GEN_SIZE});
    de::selection_strategy selectionStrategy{de::selection_strategy_best{}};
    std::shared_ptr<de::mutation_strategy> mutationStrategy = std::make_shared<de::mutation_strategy>(0.5, 0.9, DE_VAR_COUNT);
    
    de::differential_evolution de_program(DE_VAR_COUNT,
                                DE_POP_SIZE,
                                pop1,
                                pop2,
                                bestInd,
                                cons,
                                terminationStrategy,
                                selectionStrategy,
                                mutationStrategy,
                                cost_func);

    de_program.run();

    return 0;
}