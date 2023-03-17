/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-07 09:53:30
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-03-10 10:15:55
 * @Description: file content
 */
#include "de/individual.hpp"
#include "de/constrain.hpp"
namespace de {
individual::individual():
    m_vars(100)
{
    set_cost(DBL_MAX);}

individual::individual(size_t varSize) : 
    m_vars(varSize)
{set_cost(DBL_MAX);}    

individual::individual(std::vector<double>& vars) : 
    m_vars(vars)
{}

individual::individual(const individual& ind) : 
    m_vars(ind.m_vars)
{}

individual::individual(individual&& ind) :
    m_vars()
{}

void individual::init(double_constrain cons) {
    for (size_t i=0; i < m_vars.size(); i++ ){
        m_vars[i] = cons.get_rand_value();
    }
}

std::vector<double>& individual::get_vars(){
    return m_vars;
}

std::vector<int>& individual::get_taboo(){
    return m_taboo;
}

double individual::get_cost(){
    return m_cost;
}

void individual::set_cost(double cost){
    m_cost = cost;
}

double individual::operator[](size_t index){
    return m_vars[index];
}

bool individual::operator<=(const individual& ind){
    return (this -> m_cost <= ind.m_cost);
}

bool individual::operator<(const individual& ind){
    return (this -> m_cost < ind.m_cost);
}

std::ostream& operator<<(std::ostream& cout, individual& ind){
    cout << std::string("individual cost :") << ind.m_cost << "\t vars is : ";
    for (auto var : ind.m_vars){
        cout << var << " ";
    }
    return cout;
}


} // namespace de