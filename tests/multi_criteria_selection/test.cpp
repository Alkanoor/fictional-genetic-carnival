/* This test provides a proof of concept concerning multi criteria evaluation */

#include <chrono>
#include <iostream>

#include "selection_on_evaluation/multi_criteria_selection_on_evaluation.hpp"


// g++ -std=c++11 -I../../include/utils -I../.. test.cpp -o test -L../../logger/bin -llog ../../threads/thread.cpp ../../threads/thread_pool.cpp -lpthread -Wall -Werror
int main()
{
    Genotype g;
    Gene<float> cur("x", 0, 8, "NORMED_BINARY");
    g.add_gene(cur);
    cur = Gene<float>("y", 8, 16, "NORMED_BINARY");
    g.add_gene(cur);

    std::vector<char> bits;
    const std::vector<std::array<int, Population_size> >& eval_select(const std:array<std::vector<char>, Population_size>& bits, const Genotype& genes) throw() const;

    return 0;
}
