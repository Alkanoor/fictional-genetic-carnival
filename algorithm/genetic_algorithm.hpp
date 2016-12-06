#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP


///*****************************************************************************************
/// Class that provides core of genetic algorithm
/// - proba_n_crossovers : probability
/// - If cross_multiple is not set :
///    - proba_merging_n_individuals is not used : 2 individuals are merged everytime
///*****************************************************************************************

template <size_t Population_size>
class Genetic_Algorithm
{
    public:
        void generate_population();
        void evaluate_and_select();
        void mutate_and_cross_over();

        void run();

    private:
        size_t nb_to_keep;

        std::function<const std::vector<char>&(const Genotype&)> random_individual;
        std::shared_ptr<Selection_On_Evaluation<Population_size, std::array<int, Population_size> > > eval_and_select;

        std::function<void(std::vector<char>&, const Genotype&, float)> mutate;
        std::function<void(std::vector<char>&, std::vector<char>&, const Genotype&, int)> cross;
        std::function<void(std::vector<std::vector<char>&>&, const Genotype&, int)> cross_multiple;

        std::function<const std::vector<float>&(int)> proba_merging_n_individuals;
        std::function<const std::vector<float>&(int)> proba_n_crossovers;

        Genotype genes;
        std::array<std::vector<char>, Population_size> bits_of_individuals;

        float mutation_rate;
        int number_crossovers;
};

void Genetic_Algorithm::run()
{
    generate_population();
    for(int i=0; i<N_iterations;i++)
    {
        evaluate_and_select();
        mutate_and_cross_over();
    }
}


#endif
