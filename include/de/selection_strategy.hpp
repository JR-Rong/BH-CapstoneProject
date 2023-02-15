/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-20 10:21:52
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-02-14 10:10:07
 * @Description: file content
 */

#ifndef SELECTION_STRATEGY_HPP_
#define SELECTION_STRATEGY_HPP_

#include "de/population.hpp"
#include <functional>

namespace de {

using selection_strategy = std::function<void(population& pop1, population& pop2, std::shared_ptr<individual>& bestInd)>;

class selection_strategy_best {
public:
    void operator()(population& pop1, population& pop2,
                    std::shared_ptr<individual>& bestInd) {

    sort_across(pop1, pop2);

    bestInd = pop1[0];
    }

 private:
  class individual_compare {
   public:
    individual_compare(){}

    bool operator()(std::shared_ptr<individual> ind1, std::shared_ptr<individual> ind2) {
      return *ind1 < *ind2;
    }
  };

  void sort_across(population& v1, population& v2) {
    v1.insert(v1.end(), v2.begin(), v2.end());
    v2.clear();

    std::sort(v1.begin(), v1.end(), individual_compare());
    v2.insert(v2.end(), v1.begin() + v1.size() / 2, v1.end());
    v1.erase(v1.begin() + v1.size() / 2, v1.end());
  }
};

} //namespace de 

#endif // SELECTION_STRATEGY_HPP_