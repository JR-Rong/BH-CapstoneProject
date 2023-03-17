/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-08 10:28:40
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-01-15 13:06:32
 * @Description: file content
 */

#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <memory>

#include "de/individual.hpp"
#include "de/constrain.hpp"

namespace de{

class population : public std::vector<std::shared_ptr<individual>> {

private:
    size_t m_popsize;

public: 
    population(size_t popSize, size_t varCount);
    population(size_t popSize, size_t varCount, constrains<double_constrain> cons);

    void init(size_t popSize, size_t varCount);
    void set_best(std::shared_ptr<individual> best);
    std::shared_ptr<individual> best();
    friend std::ostream& operator<<(std::ostream& cout, population &pop);
};

} // namespace de

#endif //POPULATION_HPP_