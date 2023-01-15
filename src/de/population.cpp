/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-08 11:04:42
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-01-15 12:59:29
 * @Description: file content
 */

#include "de/population.hpp"

namespace de {

population::population(size_t popSize, size_t varCount)
    : m_popsize(popSize)
    , std::vector<std::shared_ptr<individual>>(popSize)
{
    init(popSize, varCount);
}

population::population(size_t popSize, size_t varCount, constrains<double_constrain> cons)
    : m_popsize(popSize)
    , std::vector<std::shared_ptr<individual>>(popSize)
{
    init(popSize, varCount);
    for (size_t i = 0; i < size(); ++i) {
        at(i)->init(cons[i]);
    }
}

void population::init(size_t popSize, size_t varCount) {
    for (size_t i = 0; i < popSize; ++i){
        operator[](i) = std::make_shared<individual>(varCount);
    }
}

std::shared_ptr<individual> population::best(){
    size_t best_ = 0;

    for (size_t i = 0; i < size(); ++i)
        best_ = at(i) <= at(best_) ? i : best_;

    return at(best_);
}


std::ostream& operator<<(std::ostream& cout, population &pop){
    int i=0;
    for(auto ind : pop){
        cout << "ind " << i <<"\t: " << *(pop.at(i++)) << std::endl; 
    }
    cout << "best ind : " << *(pop.best()) << std::endl; 
    return cout;
}

} // namespace de 