/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-09 10:15:35
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2022-12-20 16:59:03
 * @Description: file content
 */
#ifndef MUTATION_STRATEGY_HPP_
#define MUTATION_STRATEGY_HPP_

#include <random>
#include <ctime>

#include "de/individual.hpp"
#include "de/population.hpp"

namespace de {

enum mutation_strategy_enum {
    STRA_1,
    STRA_2,
    STRA_3,
    STRA_4,
    STRA_5
};

inline double genrand(double min = 0, double max = 1) {
    std::default_random_engine generator(time(nullptr));
    std::uniform_real_distribution<double> rand(min, max);
    return rand(generator);
}

inline int genintrand(int min, int max, bool upperexclusive = false) {
    int ret = 0;
    std::default_random_engine generator(time(nullptr));
    std::uniform_int_distribution<int> rand(min, max);

    do
    ret = rand(generator);
    while (ret < min || ret > max || upperexclusive && ret == max);
    return ret;
}

class Urn {
size_t m_urn[4];

public:
Urn(size_t NP, size_t avoid) {
    do {
    m_urn[0] = genintrand(0, NP, true);
    }
    while (m_urn[0] == avoid);

    do {
    m_urn[1] = genintrand(0, NP, true);
    }
    while (m_urn[1] == m_urn[0] || m_urn[1] == avoid);

    do {
    m_urn[2] = genintrand(0, NP, true);
    }
    while (m_urn[2] == m_urn[1] || m_urn[2] == m_urn[0] || m_urn[2] == avoid);

    do {
    m_urn[3] = genintrand(0, NP, true);
    }
    while (m_urn[3] == m_urn[2] || m_urn[3] == m_urn[1] || m_urn[3] == m_urn[0] || m_urn[3] == avoid);
}

size_t operator[](size_t index) const {
    return m_urn[index];
}
};

class mutation_strategy {
private:
    double m_weight;
    double m_crossover;
    double m_dither;
    size_t m_varCount;
    mutation_strategy_enum m_enum;

public:
    mutation_strategy(double weight, double crossover, size_t varCount);

    double get_weight();
    double get_crossover();
    double get_dither();
    size_t get_varCount();
    void set_strategy(mutation_strategy_enum strategy_enum);

    std::pair<std::shared_ptr<individual>, std::vector<double>> operator()(
        const population& pop,
        std::shared_ptr<individual> bestIt,
        size_t i);
};

} // namespace de 

#endif // MUTATION_STRATEGY_HPP_