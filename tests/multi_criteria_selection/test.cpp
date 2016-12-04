/* This test provides a proof of concept concerning multi criteria evaluation */

#include <chrono>
#include <iostream>

#define POPULATION_SIZE 10


#include "selection_on_evaluation/include/multi_criteria_selection_on_evaluation.hpp"
#include "selection_on_evaluation/include/simple_selection_on_evaluation.hpp"
#include "selection/include/absolute_selection.hpp"
#include "genetic/include/genotype.hpp"
#include "utils/util.hpp"


float eval1(float x, float y)
{
    return 1.0/((0.1*fabs(x-128)+0.1*fabs(y-128))*(0.05*fabs(x-64)+0.05*fabs(y-64))+0.000001);
}

float eval2(float x, float y)
{
    return 1.0/(0.1*fabs(x-128)*(0.05*fabs(x-64)+0.05*fabs(y-64))+0.000001);
}

float eval3(float x, float y)
{
    return 1.0/(0.1*fabs(x-128)*0.05*fabs(y-64)+0.000001);
}

float eval(const std::vector<char>& bits, const Genotype& g)
{
    g.interprete(bits);
    float a = g.get_gene_float("x");
    float b = g.get_gene_float("y");
    g.get_gene_int("x"); // throws
    g.get_gene_float("zzz"); // throws
    return a+b;
}

// g++ -std=c++11 -I../.. test.cpp -o test -L../../logger/bin -llog ../../threads/thread.cpp ../../threads/thread_pool.cpp -lpthread -Wall -Werror
int main()
{
    Genotype g;
    Gene<float> cur("x", 0, 8, "BINARY");
    g.add_gene(cur);
    cur = Gene<float>("y", 8, 16, "BINARY");
    g.add_gene(cur);

    std::array<std::vector<char>, POPULATION_SIZE> bits;
    bits[0] = {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0};
    bits[1] = {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0};
    bits[2] = {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1};
    bits[3] = {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1};
    bits[4] = {0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0};
    bits[5] = {0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0};
    bits[6] = {0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,1};
    bits[7] = {0,0,0,1,1,1,1,1,0,0,1,1,1,1,0,1};
    bits[8] = {1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1};
    bits[9] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};


    std::function<float(const std::vector<char>&, const Genotype&)> eval = std::bind(eval, std::placeholders::_1, std::placeholders::_2);

    auto select0 = std::make_shared<Absolute_Selection<float, POPULATION_SIZE, 3> >();
    auto select1 = std::make_shared<Absolute_Selection<float, POPULATION_SIZE, 3> >(1);
    auto select2 = std::make_shared<Absolute_Selection<float, POPULATION_SIZE, 3> >(2);

    Multi_Criteria_Selection_On_Evaluation<POPULATION_SIZE> multi_criteria_selection;

    auto selection_on_evaluation0 = std::make_shared<Simple_Selection_On_Evaluation<POPULATION_SIZE, float, 3> >(eval, select0);
    auto selection_on_evaluation1 = std::make_shared<Simple_Selection_On_Evaluation<POPULATION_SIZE, float, 3> >(eval, select1);
    auto selection_on_evaluation2 = std::make_shared<Simple_Selection_On_Evaluation<POPULATION_SIZE, float, 3> >(eval, select2);

    multi_criteria_selection.add_selection_on_evaluation(selection_on_evaluation0);
    multi_criteria_selection.add_selection_on_evaluation(selection_on_evaluation1);
    multi_criteria_selection.add_selection_on_evaluation(selection_on_evaluation2);

    std::vector<std::array<int, POPULATION_SIZE> > final_result = multi_criteria_selection.eval_select(bits, g);

    for(auto v : final_result)
        Utils::write_vec(v, std::cout, ' ', true);

    return 0;
}
