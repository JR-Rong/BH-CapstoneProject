/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-09 11:38:17
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-03-08 13:52:15
 * @Description: file content
 */

#include "de/mutation_strategy.hpp"

namespace de {

mutation_strategy::mutation_strategy(double weight, double crossover, size_t varCount)
    : m_weight(weight)
    , m_crossover(crossover)
    , m_dither(weight + genrand() * (1.0 - weight))
    , m_varCount(varCount)
    {
        m_enum = mutation_strategy_enum::STRA_1;
    }

double mutation_strategy::get_weight() {
    return m_weight;
}
double mutation_strategy::get_crossover() {
    return m_crossover;
}
double mutation_strategy::get_dither() {
    return m_dither;
}
size_t mutation_strategy::get_varCount() {
    return m_varCount;
}

void mutation_strategy::set_strategy(mutation_strategy_enum strategy_enum){
    m_enum = strategy_enum;
}


std::pair<std::shared_ptr<individual>, std::vector<double>> mutation_strategy::operator()(
    const population& pop,
    std::shared_ptr<individual> bestIt,
    size_t i) {

    std::shared_ptr<individual> tmpInd(std::make_shared<individual>(pop[i]->get_vars()));
    const Urn urn(pop.size(), i);

    // make sure j is within bounds
    size_t j = genintrand(0, get_varCount(), true);
    size_t k = 0;
    double jitter = (0.0001 * genrand() + get_weight());
    double val = 0;
    
    do {
        switch (m_enum)
        {
        case STRA_1:
            tmpInd->get_vars().at(j) = pop[urn[0]]->get_vars().at(j) + get_weight() * (pop[urn[1]]->get_vars().at(j) - pop[urn[2]]->get_vars().at(j));
            break;
        case STRA_2:
            tmpInd->get_vars().at(j) = tmpInd->get_vars().at(j) + get_weight() * (bestIt->get_vars().at(j) - tmpInd->get_vars().at(j)) +
                get_weight() * (pop[urn[1]]->get_vars().at(j) - pop[urn[2]]->get_vars().at(j));
            break;
        case STRA_3:
            (tmpInd->get_vars())[j] =
                (bestIt->get_vars())[j] +
                jitter * ((pop[urn[1]]->get_vars())[j] - (pop[urn[2]]->get_vars())[j]);
            break;
        case STRA_4:
            (tmpInd->get_vars())[j] =
                (pop[urn[0]]->get_vars())[j] +
                (get_weight() + genrand() * (1.0 - get_weight())) * ((pop[urn[1]]->get_vars())[j] - (pop[urn[2]]->get_vars())[j]);
            break;
        case STRA_5:
            tmpInd->get_vars().at(j) = pop[urn[0]]->get_vars().at(j) + get_weight() * (pop[urn[1]]->get_vars().at(j) - pop[urn[2]]->get_vars().at(j));
            break;
        default:
            tmpInd->get_vars().at(j) = pop[urn[0]]->get_vars().at(j) + get_weight() * (pop[urn[1]]->get_vars().at(j) - pop[urn[2]]->get_vars().at(j));
            break;
        }
        // if(find(tmpInd -> get_taboo().begin(), tmpInd -> get_taboo().end(), static_cast<int>(tmpInd->get_vars().at(j))) == tmpInd -> get_taboo().end()) {
        //     j = ++j % get_varCount();
        //     ++k;
        //     tmpInd -> get_taboo().push_back(static_cast<int>(tmpInd->get_vars().at(j)));
        // }
        j = ++j % get_varCount();
        ++k;
    } while (genrand() < get_crossover() && k < get_varCount());

    std::vector<double> origin(pop[urn[0]]->get_vars());

    return {tmpInd, origin};
}



} // namespace de 