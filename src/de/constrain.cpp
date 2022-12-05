/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-05 17:35:09
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2022-12-05 21:28:42
 * @Description: file content
 */

#include "de/constrain.hpp"

de::double_constrain::double_constrain(double max = 100, 
                    double min = 1,
                    int randomseed = 1000) :
                    m_max(max),
                    m_min(min)
{
    m_generator.seed(randomseed);
    m_rand = std::uniform_real_distribution<double>(m_min, m_max);
}

double de::double_constrain::get_rand_value() {
    return this -> m_rand(m_generator);
}

double de::double_constrain::get_rand_value(double value, double origin) {
    std::uniform_real_distribution<double> rand(0, 1);
    
    double ret = value;

    while (ret < m_min) {
      ret = m_min + rand(m_generator) * (origin - m_min);
    }

    while (ret > m_max) {
      ret = m_max + rand(m_generator) * (origin - m_max);
    }

    return ret;
}

double de::double_constrain::get_middle_point() {
    return (m_max + m_min) / 2;
}

double de::double_constrain::get_max(){
    return m_max;
}

double de::double_constrain::get_min(){
    return m_min;
}