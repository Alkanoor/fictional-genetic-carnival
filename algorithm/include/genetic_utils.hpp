#ifndef GENETIC_UTILS_HPP
#define GENETIC_UTILS_HPP


#include "genetic/include/genotype.hpp"


class Genetic_Utils
{
    public:
        Genetic_Utils(int merge_individuals_size = 4, int n_crossovers_size = 8);

        const std::vector<char>& basic_random_individual_0_1(const Genotype& genes);
        const std::vector<char>& basic_random_individual_0_255(const Genotype& genes);

        static void static_basic_random_individual_0_1(const Genotype& genes, std::vector<char>& ret);
        static void static_basic_random_individual_0_255(const Genotype& genes, std::vector<char>& ret);

        static void mutate_basic_uniform(std::vector<char>& bits, const Genotype& genes, float mutation_rate, int min=0, int max=1);
        static void mutate_basic_per_gene(std::vector<char>& bits, const Genotype& genes, float mutation_rate, int min=0, int max=1);
        static void mutate_basic_uniform_flip(std::vector<char>& bits, const Genotype& genes, float mutation_rate);
        static void mutate_basic_per_gene_flip(std::vector<char>& bits, const Genotype& genes, float mutation_rate);

        static void cross_basic(std::vector<char>& bits1, std::vector<char>& bits2, const Genotype& genes, int nb_cuts);
        static void cross_multiple_cyclic(std::vector<std::vector<char>*>& bits, const Genotype& genes, int nb_cuts);

        const std::vector<float>& basic_proba_merging_n_individuals(float ratio);
        const std::vector<float>& basic_proba_n_crossovers(int min_rank);

    private:
        std::vector<float> proba_merging_n_individuals;
        std::vector<float> proba_n_crossovers;
        std::vector<char> individual;

        int prev_min_rank;
        float prev_ratio;
};


#endif
