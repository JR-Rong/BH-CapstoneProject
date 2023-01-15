/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-20 10:13:24
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2022-12-26 15:19:49
 * @Description: file content
 */
#include <functional>

namespace de {

using termination_strategy = std::function<bool(size_t)>;

class termination_strategy_max_gen {
private:
    size_t m_maxgen;

public:
    termination_strategy_max_gen(size_t max) :
        m_maxgen(max)
    {}

    bool operator()(size_t genCount){
        return genCount <= m_maxgen;
    }
};

} // namespace de 