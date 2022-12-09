/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-08 10:28:40
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2022-12-09 12:06:05
 * @Description: file content
 */

#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <memory>

#include "de/individual.hpp"

namespace de{

class population : public std::vector<std::shared_ptr<individual>> {

private:
    size_t m_popsize;

public: 
    population(size_t popSize, size_t varCount);
    population(size_t popSize, size_t varCount, double_constrain cons);

    void init(size_t popSize, size_t varCount);
    std::shared_ptr<individual> best();
    friend std::ostream& operator<<(std::ostream& cout, population &pop);
};

} // namespace de

#endif //POPULATION_HPP_