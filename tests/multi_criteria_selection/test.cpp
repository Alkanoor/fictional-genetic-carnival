/* This test provides a proof of concept concerning multi criteria evaluation */

#include <chrono>
#include <iostream>

#define POPULATION_SIZE 20000


#include "selection_on_evaluation/include/multi_criteria_selection_on_evaluation.hpp"
#include "selection_on_evaluation/include/simple_selection_on_evaluation.hpp"
#include "selection/include/absolute_selection.hpp"
#include "genetic/include/genotype.hpp"
#include "utils/util.hpp"


float eval1(int x, int y)
{
    return 1.0/((0.1*fabs(x-128)+0.1*fabs(y-128))*(0.05*fabs(x-64)+0.05*fabs(y-64))+0.000001);
}

float eval2(int x, int y)
{
    return 1.0/(0.1*fabs(x-128)*(0.05*fabs(x-64)+0.05*fabs(y-64))+0.000001);
}

float eval3(int x, int y)
{
    return 1.0/(0.1*fabs(x-128)*0.05*fabs(y-64)+0.000001);
}

float eval(const std::vector<char>& bits, Genotype& g, const std::function<float(int, int)>& eval_f)
{
    g.interprete(bits);
    int a = g.get_gene_int("x").get_current_interpretation();
    int b = g.get_gene_int("y").get_current_interpretation();
    //g.get_gene_float("x"); // throws
    //g.get_gene_int("zzz"); // throws
    //std::cout<<a<<" , "<<b<<" => "<<eval_f(a,b)<<std::endl;
    return eval_f(a,b);
}

// g++ -std=c++14 -I../.. test.cpp $(echo $(ls ../../threads/*.cpp)) $(echo $(ls ../../logger/*.cpp)) $(echo $(ls ../../genetic/*.cpp)) ../../utils/utils.cpp -o test -Wall -Werror -lpthread -g
int main()
{
    Genotype g;
    Gene<int> cur_x("x", 0, 8, "BINARY");
    g.add_gene(cur_x);
    Gene<int> cur_y("y", 8, 16, "BINARY");
    g.add_gene(cur_y);

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
    for(int i=10; i<POPULATION_SIZE; i++)
    {
        bits[i] = std::vector<char>(16);
        for(int j=0; j<16; j++)
            bits[i][j] = rand()%2;
    }

    std::function<float(int, int)> f1 = std::bind(eval1, std::placeholders::_1, std::placeholders::_2);
    std::function<float(int, int)> f2 = std::bind(eval2, std::placeholders::_1, std::placeholders::_2);
    std::function<float(int, int)> f3 = std::bind(eval3, std::placeholders::_1, std::placeholders::_2);

    std::function<float(const std::vector<char>&, Genotype&)> eval_function1 = std::bind(eval, std::placeholders::_1, std::placeholders::_2, f1);
    std::function<float(const std::vector<char>&, Genotype&)> eval_function2 = std::bind(eval, std::placeholders::_1, std::placeholders::_2, f2);
    std::function<float(const std::vector<char>&, Genotype&)> eval_function3 = std::bind(eval, std::placeholders::_1, std::placeholders::_2, f3);

    auto select0 = std::make_shared<Absolute_Selection<POPULATION_SIZE, float, 3> >();
    auto select1 = std::make_shared<Absolute_Selection<POPULATION_SIZE, float, 3> >(1);
    auto select2 = std::make_shared<Absolute_Selection<POPULATION_SIZE, float, 3> >(2);

    Multi_Criteria_Selection_On_Evaluation<POPULATION_SIZE> multi_criteria_selection;

    auto selection_on_evaluation0 = std::make_shared<Simple_Selection_On_Evaluation<POPULATION_SIZE, float, 3> >(eval_function1, select0);
    auto selection_on_evaluation1 = std::make_shared<Simple_Selection_On_Evaluation<POPULATION_SIZE, float, 3> >(eval_function2, select1);
    auto selection_on_evaluation2 = std::make_shared<Simple_Selection_On_Evaluation<POPULATION_SIZE, float, 3> >(eval_function3, select2);

    multi_criteria_selection.add_selection_on_evaluation(selection_on_evaluation0);
    multi_criteria_selection.add_selection_on_evaluation(selection_on_evaluation1);
    multi_criteria_selection.add_selection_on_evaluation(selection_on_evaluation2);

    //comment next lines to see performance changes
    Thread t1(1);
    Thread t2(2);
    //On telecom machine : 1.3 s to perform with 1 thread, 1 s to perform with 2, and 0.72 with 3
    //On my machine : increasing number of threads decreases performance (a bit, but it should at least be the contrary)

    auto start = std::chrono::system_clock::now();
    std::vector<std::array<int, POPULATION_SIZE> > final_result = multi_criteria_selection.eval_select(bits, g);
    auto end = std::chrono::system_clock::now();

    std::cout<<"Computation took "<<(end-start).count()/1000<<" microseconds "<<std::endl;

    /*for(auto v : final_result)
        Utils::write_vec(v, std::cout, ' ')<<std::endl;*/

    return 0;
}
