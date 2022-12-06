/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-05 17:29:41
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2022-12-06 22:50:38
 * @Description: file content
 */
#ifndef CONSTRAIN_HPP_
#define CONSTRAIN_HPP_

#include <random>

namespace de {

class constrain{

public :
    virtual double get_rand_value() = 0;
    virtual double get_rand_value(double value, double origin) = 0;
    virtual double get_middle_point() = 0;
    virtual double get_min() = 0;
    virtual double get_max() = 0;
};

class double_constrain : constrain{
private: 
    double m_max;
    double m_min;
protected:
    std::uniform_real_distribution<double> m_rand;
    std::default_random_engine m_generator;
public :
    double_constrain(double max, 
                    double min,
                    int randomseed);

    virtual double get_rand_value() override;
    virtual double get_rand_value(double value, double origin) override;
    virtual double get_middle_point() override;
    virtual double get_min() override;
    virtual double get_max() override;
};

} // namespace de

#endif // CONSTRAIN_HPP_