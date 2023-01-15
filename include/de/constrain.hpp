/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-05 17:29:41
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-01-15 13:27:24
 * @Description: file content
 */
#ifndef CONSTRAIN_HPP_
#define CONSTRAIN_HPP_

#include <random>
#include <time.h> 

namespace de {
class individual;
class population;
class constrain_base{

public :
    virtual double get_rand_value() = 0;
    virtual double get_rand_value(double value, double origin) = 0;
    virtual double get_middle_point() = 0;
    virtual double get_min() = 0;
    virtual double get_max() = 0;
};

class double_constrain : constrain_base{
private: 
    double m_max;
    double m_min;
    size_t times;
protected:
    std::uniform_real_distribution<double> m_rand;
    std::default_random_engine m_generator;
public :
    double_constrain(double max, 
                    double min,
                    unsigned int randomseed);

    virtual double get_rand_value() override;
    virtual double get_rand_value(double value, double origin) override;
    virtual double get_middle_point() override;
    virtual double get_min() override;
    virtual double get_max() override;
    void ensure_constrain(individual& ind);
};

template <typename constrain>
class constrains : public std::vector<constrain> {
public:
    constrains(double max, double min,unsigned int randomseed, size_t ind_num){
        for(int i=0;i< ind_num;i++){
            constrain cons{max, min, randomseed+i};
            this -> emplace_back(cons);
        }
    }

    void ensure_constrains(population& pop){
        for(int i=0; i<pop.size(); i++){
        auto ind = *(pop[i]);
        constrain con = at(i);
        con.ensure_constrain(ind);
        }
    }

    void ensure_constrains(individual& ind, size_t idx){
        constrain con = at(idx);
        con.ensure_constrain(ind);
    }
};

} // namespace de

#endif // CONSTRAIN_HPP_