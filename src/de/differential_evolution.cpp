/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2022-12-20 11:05:11
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-01-15 13:27:10
 * @Description: file content
 */
#include "de/differential_evolution.hpp"

namespace de {
    
    differential_evolution::differential_evolution(size_t varCount,
                                                    size_t popSize,
                                                    population pop1,
                                                    population pop2,
                                                    std::shared_ptr<individual> bestInd,
                                                    constrains<double_constrain>& constraint,
                                                    termination_strategy& terminationStrategy,
                                                    selection_strategy& selectionStrategy,
                                                    std::shared_ptr<mutation_strategy> mutationStrategy,
                                                    std::function<double(std::shared_ptr<de::individual>&)> costFunc) : 
            m_varCount(varCount),
            m_popSize(popSize),
            m_pop1(pop1),
            m_pop2(pop2),
            m_bestInd(bestInd),
            m_constraints(constraint),
            m_terminationStrategy(terminationStrategy),
            m_selectionStrategy(selectionStrategy),
            m_mutationStrategy(mutationStrategy),
            m_costFunc(costFunc)
        {}

    std::shared_ptr<individual> differential_evolution::get_best(){
        return m_bestInd;
    }

    void differential_evolution::run(){
        
        utils::Logger logger;
        std::shared_ptr<individual> bestInd(m_bestInd);
        for(size_t gen=0; std::invoke(m_terminationStrategy, gen); gen++){
            // std::cout << "--------- New Gen Start, Gen count:" << gen << " ----------" << std::endl;
            // logger << "--------- New Gen Start, Gen count:" << gen << " ----------";
            // logger << "+++++++++ Start mutation ++++++++++";
            // logger << "ind|";
            std::cout << "--------- New Gen Start, Gen count:" << gen << " ----------" << std::endl;
            std::cout << "+++++++++ Start mutation ++++++++++"<< std::endl;
            std::cout << "ind|";
            int last_val = 0;
            for(size_t indcount=0; indcount < m_popSize; indcount++){
                // std::cout << ">";
                std::pair<std::shared_ptr<individual>, std::vector<double>> mutation_info((*m_mutationStrategy)(m_pop1, bestInd, indcount));
                m_constraints.ensure_constrains(*(mutation_info.first), indcount);
                mutation_info.first -> set_cost(m_costFunc(mutation_info.first));
                logger <<  *mutation_info.first;
                m_pop2.at(indcount) = mutation_info.first;
            }
            std::cout << std::endl;
            std::cout  << "+++++++++ end mutation ++++++++++"<< std::endl;
            std::cout  << "+++++++++ start selection ++++++++++"<< std::endl;
            std::invoke(m_selectionStrategy, m_pop1, m_pop2, m_bestInd);
            std::cout  << "+++++++++ end selection ++++++++++"<< std::endl;
            std::cout  << "bestInd :" << m_pop1<< std::endl;
        }
        logger.stroe_log();
    }

} //namespace de