/* This test provides a proof of concept concerning multi criteria evaluation */

#include <chrono>
#include <iostream>

#define POPULATION_SIZE 150
#define N_ITERATIONS 100
#define MUTATION_RATE 0.07
#define N_BITS 2000


#include "selection_on_evaluation/include/multi_criteria_selection_on_evaluation.hpp"
#include "selection_on_evaluation/include/simple_selection_on_evaluation.hpp"
#include "selection/include/quality_selection.hpp"
#include "algorithm/include/genetic_algorithm.hpp"
#include "utils/util.hpp"

#include <sstream>


int eval(const std::vector<char>& bits, Genotype& g)
{
    int s = 0;
    g.interprete(bits);
    for(int i=0; i<N_BITS; i++)
        s += g.get_gene_int(i).get_current_interpretation();
    return s;
}

int main()
{
    Genotype genes;
    for(int i=0; i<N_BITS; i++)
    {
        std::ostringstream oss;
        oss<<"bit"<<i;
        Gene<int> cur(oss.str(), i, i+1, "BINARY");
        genes.add_gene(cur);
    }

    std::function<float(const std::vector<char>&, Genotype&)> eval_function = std::bind(eval, std::placeholders::_1, std::placeholders::_2);
    auto select = std::make_shared<Quality_Selection<POPULATION_SIZE, float, 3> >();
    auto simple_sum = std::make_shared<Simple_Selection_On_Evaluation<POPULATION_SIZE, float, 3> >(eval_function, select);
    Genetic_Algorithm<POPULATION_SIZE, float> ga(N_ITERATIONS, 50, MUTATION_RATE, genes, simple_sum);

    //comment next lines to see performance changes
    //Thread t1(1);
    //Thread t2(2);

    auto start = std::chrono::system_clock::now();
    ga.run();
    auto end = std::chrono::system_clock::now();

    std::cout<<"Computation took "<<(end-start).count()/1000<<" microseconds "<<std::endl;

    //Genetic_Algorithm<150, float> ga_complex(100, 50, 0.07, genes, multi_optimums);

    return 0;
}
