/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-06 22:47:52
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-01-15 13:23:36
 * @Description: file content
 */
#ifndef INDIVIDUAL_HPP_
#define INDIVIDUAL_HPP_

#include <vector>
#include <iostream>

namespace de {
class double_constrain;
class individual{
public:
    individual();
    individual(size_t varSize);
    individual(std::vector<double>& vars);
    individual(const individual& ind);
    individual(individual&& ind);

    void init(double_constrain cons);
    std::vector<double>& get_vars();
    double get_cost();
    void set_cost(double cost);
    double operator[](size_t index);
    bool operator<=(const individual& ind);
    bool operator<(const individual& ind);
    friend std::ostream& operator<<(std::ostream& cout, individual& ind);

private:
    std::vector<double> m_vars;
    double m_cost;
};

} //namespace de

#endif // INDIVIDUAL_HPP_