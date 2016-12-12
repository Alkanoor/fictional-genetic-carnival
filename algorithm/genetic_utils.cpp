#include "include/genetic_utils.hpp"


const std::vector<char>& Genetic_Utils::basic_random_individual_0_1(const Genotype& genes)
{
    individual.resize(genes.get_max_location());
    int s = (int)individual.size();
    for(int i=0; i<s; i++)
        individual[i] = rand()%2;
    return individual;
}

const std::vector<char>& Genetic_Utils::basic_random_individual_0_255(const Genotype& genes)
{
    individual.resize(genes.get_max_location());
    int s = (int)individual.size();
    for(int i=0; i<s; i++)
        individual[i] = rand()%256;
    return individual;
}


void Genetic_Utils::mutate_basic_uniform(std::vector<char>& bits, const Genotype& genes, float mutation_rate, int min, int max)
{
    int n_mutations_per_10000000 = mutation_rate*10000000;
    assert(n_mutations>0 && max>min);

    int s = bits.size();
    for(int i=0; i<s; i++)
        if(rand()%10000000 < n_mutations_per_10000000)
            bits[i] = rand()%(max-min+1);
}

void Genetic_Utils::mutate_basic_per_gene(std::vector<char>& bits, const Genotype& genes, float mutation_rate, int min, int max)
{
    int n_mutations_per_10000000 = mutation_rate*10000000;
    assert(n_mutations>0 && max>min);

    size_t n_integer_genes = genes.get_number_integer_genes();
    size_t n_float_genes = genes.get_number_float_genes();

    for(int i=0; i<(int)n_integer_genes; i++)
    {
        int m=genes.get_gene_int(i).get_min_location();
        int M=genes.get_gene_int(i).get_max_location();
        for(int j=m; j<M; j++)
            if(rand()%10000000 < n_mutations_per_10000000)
            {
                bits[j] = rand()%(max-min+1);
                break;
            }
    }

    for(int i=0; i<(int)n_float_genes; i++)
    {
        int m=genes.get_gene_float(i).get_min_location();
        int M=genes.get_gene_float(i).get_max_location();
        for(int j=m; j<M; j++)
            if(rand()%10000000 < n_mutations_per_10000000)
            {
                bits[j] = rand()%(max-min+1);
                break;
            }
    }
}

void Genetic_Utils::mutate_basic_uniform_flip(std::vector<char>& bits, const Genotype& genes, float mutation_rate);
{
    int n_mutations_per_10000000 = mutation_rate*10000000;
    assert(n_mutations>0);

    int s = bits.size();
    for(int i=0; i<s; i++)
        if(rand()%10000000 < n_mutations_per_10000000)
            bits[i] = bits[i]^1;
}

void Genetic_Utils::mutate_basic_per_gene_flip(std::vector<char>& bits, const Genotype& genes, float mutation_rate);
{
    int n_mutations_per_10000000 = mutation_rate*10000000;
    assert(n_mutations>0);

    size_t n_integer_genes = genes.get_number_integer_genes();
    size_t n_float_genes = genes.get_number_float_genes();

    for(int i=0; i<(int)n_integer_genes; i++)
    {
        int m=genes.get_gene_int(i).get_min_location();
        int M=genes.get_gene_int(i).get_max_location();
        for(int j=m; j<M; j++)
            if(rand()%10000000 < n_mutations_per_10000000)
            {
                bits[j] = bits[i]^1;
                break;
            }
    }

    for(int i=0; i<(int)n_float_genes; i++)
    {
        int m=genes.get_gene_float(i).get_min_location();
        int M=genes.get_gene_float(i).get_max_location();
        for(int j=m; j<M; j++)
            if(rand()%10000000 < n_mutations_per_10000000)
            {
                bits[j] = bits[i]^1;
                break;
            }
    }
}


void Genetic_Utils::cross_basic(std::vector<char>& bits1, std::vector<char>& bits2, const Genotype& genes, int nb_cuts)
{
    size_t n_integer_genes = genes.get_number_integer_genes();
    size_t n_float_genes = genes.get_number_float_genes();
    std::ordered_set<int> cuts;
    int cur_cut;
    for(int i=0; i<nb_cuts; i++)
    {
        int chosen = rand()%(n_integer_genes+n_float_genes);
        if(chosen<n_integer_genes)
            if(rand()%2)
                cur_cut = genes.get_gene_int(chosen).get_min_location();
            else
                cur_cut = genes.get_gene_int(chosen).get_max_location();
        else
            if(rand()%2)
                cur_cut = genes.get_gene_float(chosen-n_integer_genes).get_min_location();
            else
                cur_cut = genes.get_gene_float(chosen-n_integer_genes).get_max_location();
        cuts.insert(cur_cut);
    }

    bool type = rand()%2;

    int a = 0, n = 0;
    if(type)
        for(int b : cuts)
        {
            if(n%2)
                for(int i=a; i<b; i++)
                {
                    int tmp = bits1[i];
                    bits1[i] = bits2[i];
                    bits2[i] = tmp;
                }
            a = b;
            n++;
        }
    else
        for(int b : cuts)
        {
            if(!(n%2))
                for(int i=a; i<b; i++)
                {
                    int tmp = bits1[i];
                    bits1[i] = bits2[i];
                    bits2[i] = tmp;
                }
            a = b;
            n++;
        }
}

void Genetic_Utils::cross_multiple_cyclic(std::vector<std::vector<char>&>& bits, const Genotype& genes, int nb_cuts)
{}


const std::vector<float>& Genetic_Utils::basic_proba_merging_n_individuals(float ratio)
{
    if(ratio>0.5)
    {
        proba_merging_n_individuals[0] = 0.24;
        proba_merging_n_individuals[1] = 0.6;
        proba_merging_n_individuals[2] = 0.15;
        proba_merging_n_individuals[3] = 0.01;
    }
    else if(ratio>0.25)
    {
        proba_merging_n_individuals[0] = 0.15;
        proba_merging_n_individuals[1] = 0.55;
        proba_merging_n_individuals[2] = 0.2;
        proba_merging_n_individuals[3] = 0.1;
    }
    else
    {
        proba_merging_n_individuals[0] = 0.1;
        proba_merging_n_individuals[1] = 0.55;
        proba_merging_n_individuals[2] = 0.2;
        proba_merging_n_individuals[3] = 0.15;
    }
    return proba_merging_n_individuals;
}

const std::vector<float>& Genetic_Utils::basic_proba_n_crossovers(int min_rank);
{
    return proba_n_crossovers;
}
