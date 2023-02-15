/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-05 17:34:57
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-02-14 19:55:08
 * @Description: file content
 */
#include <iostream>

#include "de/differential_evolution.hpp"
#include "mapping/mapping.hpp"

#define DE_VAR_COUNT 8
#define DE_POP_SIZE 15
#define DE_GEN_SIZE 50

double cost_fun(std::shared_ptr<de::individual>& ind_ptr){
    double res = 0;
    for(auto var:ind_ptr->get_vars()){
        res += var * var;
    }
    return res;
}

int main(){

    mapping::display_map map;
    
    if(mapping::choose_build_map_way()){
        map.input_size();
        map.init();
        map.gen_map();
        map.display();
        mapping::store_map(map);
    } else {
        #ifdef PROJECT_CURRENT_DIR
            std::string map_file = std::string(PROJECT_CURRENT_DIR) + "map_build\\map_default.json";
            mapping::read_map(map_file, map);
        #endif
        map.init();
        map.display();
    }
    
    de::constrains<de::double_constrain> cons(100, 1, (unsigned)time( NULL ), DE_POP_SIZE);
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

    // de_program.run();

    return 0;
}