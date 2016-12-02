/* This test provides a proof of concept concerning multi criteria evaluation */

#include <chrono>
#include <iostream>

#define POPULATION_SIZE 10


#include "selection_on_evaluation/multi_criteria_selection_on_evaluation.hpp"


float eval(float x, float y)
{
    return 1.0/((fabs(2*x-1)-0.5)+(fabs(2*y-1)-0.5)+0.000001);
}

// g++ -std=c++11 -I../.. test.cpp -o test -L../../logger/bin -llog ../../threads/thread.cpp ../../threads/thread_pool.cpp -lpthread -Wall -Werror
int main()
{
    Genotype g;
    Gene<float> cur("x", 0, 8, "NORMED_BINARY");
    g.add_gene(cur);
    cur = Gene<float>("y", 8, 16, "NORMED_BINARY");
    g.add_gene(cur);

    std::array<std::vector<char>, POPULATION_SIZE> bits;
    bits[0] = {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0};
    std::cout<<Common_Interpretations::get
        const std::vector<std::array<int, Population_size> >& eval_select(const std:array<std::vector<char>, Population_size>& bits, const Genotype& genes) throw() const;

    return 0;
}
