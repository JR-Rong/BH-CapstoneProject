/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-11-29 19:56:25
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-02-14 10:09:27
 * @Description: 
 */

#ifndef DIFFERENTIAL_EVOLUTION_HPP_
#define DIFFERENTIAL_EVOLUTION_HPP_

#include <memory>
#include <vector>
#include <math.h>
#include <iostream>

#include "de/constrain.hpp"
#include "de/individual.hpp"
#include "de/population.hpp"
#include "de/mutation_strategy.hpp"
#include "de/termination_strategy.hpp"
#include "de/selection_strategy.hpp"

#include "utils/utils.hpp"

namespace de {

class differential_evolution{
private:
    size_t m_varCount;
    size_t m_popSize;

    population m_pop1;
    population m_pop2;
    std::shared_ptr<individual> m_bestInd;

    constrains<double_constrain> m_constraints;
    termination_strategy m_terminationStrategy;
    selection_strategy m_selectionStrategy;
    std::shared_ptr<mutation_strategy> m_mutationStrategy;

    std::function<double(std::shared_ptr<de::individual>&)> m_costFunc;

    
public:
    differential_evolution(size_t varCount,
        size_t popSize,
        population pop1,
        population pop2,
        std::shared_ptr<individual> bestInd,
        constrains<double_constrain>& constraints,
        termination_strategy& terminationStrategy,
        selection_strategy& selectionStrategy,
        std::shared_ptr<mutation_strategy> mutationStrategy,
        std::function<double(std::shared_ptr<de::individual>&)> costFunc);
    
    std::shared_ptr<individual> get_best();

    void run();
};

} //namespace de

#endif // DIFFERENTIAL_EVOLUTION_HPP_
