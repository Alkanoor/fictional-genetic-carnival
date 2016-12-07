#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP


///************************************************************************************************************
/// Class that provides core of genetic algorithm
///
/// - proba_n_crossovers : at index i is probability of having i+1 cut(s) in ADN during crossover
///                        (depend of rank of current considered individuals)
///                        (if not set, default is proba 1 for 1 cut in ADN)
/// - proba_merging_n_individuals : at index i is probability of merging i+1 individuals during crossover
///                                 (depend of rank of current considered individuals)
///                                 (if not set, default is proba 0.4 for 1 implied individual and
///                                                               0.6 for 2 implied individuals)
///
/// - If cross_multiple is not set :
///     proba_merging_n_individuals is not used : 1 or 2 individuals are merged everytime
/// - Else if cross_multiple is set and proba_merging_n_individuals is not :
///     cross_multiple is not used : 1 or 2 individuals are merged everytime
///
/// - mutate           is the function that performs mutations depending on genotype, bits and mutation rate
/// - cross            is the function that performs crossovers over 2 individuals depending on genotype
///                    and number cuts to perform
/// - cross_multiple   is the function that performs crossovers over more than 2 individuals depending on
///                    genotype and number cuts to perform
///************************************************************************************************************

template <size_t Population_size, size_t Max_nb_crossovers>
class Genetic_Algorithm
{
    public:
        Genetic_Algorithm(size_t N, float mutation_rate, const Genotype& genes,
                          const std::function<const std::vector<char>&(const Genotype&)>& generate,
                          const std::shared_ptr<Selection_On_Evaluation<Population_size, std::array<int, Population_size> > >& eval_select,
                          const std::function<void(std::vector<char>&, const Genotype&, float)>& mutate,
                          const std::function<void(std::vector<char>&, std::vector<char>&, const Genotype&, int)>& cross);

        void generate_population();
        void evaluate_and_select();
        void mutate_and_cross_over();

        void run();

        const Genotype& get_genes() const;
        const std::array<T, Population_size>& get_evaluated() const;
        const std::array<std::vector<char>, Population_size>& get_population() const;

    private:
        size_t nb_to_keep;

        std::function<const std::vector<char>&(const Genotype&)> random_individual;
        std::shared_ptr<Selection_On_Evaluation<Population_size, std::array<int, Population_size> > > eval_and_select;

        std::function<void(std::vector<char>&, const Genotype&, float)> mutate;
        std::function<void(std::vector<char>&, std::vector<char>&, const Genotype&, int)> cross;
        std::function<void(std::vector<std::vector<char>&>&, const Genotype&, int)> cross_multiple;

        std::function<const std::vector<float>&(int)> proba_merging_n_individuals;
        std::function<const std::array<float, Max_nb_crossovers>&(int)> proba_n_crossovers;

        Genotype genes;
        std::array<T, Population_size> evaluated;
        std::array<int, Population_size> sorted_selected;
        std::array<std::vector<char>, Population_size> bits_of_individuals;
        std::array<std::vector<char>, Population_size> next_generation;

        float mutation_rate;
        int number_crossovers;
};

Genetic_Algorithm(size_t N, float mutation_rate, const Genotype& genes,
                  const std::function<const std::vector<char>&(const Genotype&)>& generate,
                  const std::shared_ptr<Selection_On_Evaluation<Population_size, std::array<int, Population_size> > >& eval_select,
                  const std::function<void(std::vector<char>&, const Genotype&, float)>& mutate,
                  const std::function<void(std::vector<char>&, std::vector<char>&, const Genotype&, int)>& cross)
{
    assert(N>2);
    assert(Population_size>=N);
}

void Genetic_Algorithm::run()
{
    generate_population();
    for(int i=0; i<N_iterations;i++)
    {
        evaluate_and_select();
        if(hook)
            hook_data();
        mutate_and_cross_over();
    }
    evaluate_and_select();
    if(hook)
        hook_data();
    else
        evaluated = eval->eval_select(bits_of_individuals, genes);
}

void Genetic_Algorithm::generate_population()
{
    for(int i=0; i<(int)Population_size; i++)
        bits_of_individuals = random_individual(genes);
}

void  Genetic_Algorithm::evaluate_and_select()
{
    sorted_selected = eval_and_select->eval_select(bits_of_individuals, genes);
    if(hook)
        evaluated = eval->eval_select(bits_of_individuals, genes);
}

void  Genetic_Algorithm::mutate_and_cross_over()
{
    for(int i=0; i<nb_to_keep; i++)
        next_generation[i] = bits_of_individuals[sorted_selected[i]];

    int n_cuts = 1, n_choices = 1;
    for(int i=nb_to_keep; i<Population_size;)
    {
        if(cross_multiple && proba_merging_n_individuals)
        {
            auto probas = proba_merging_n_individuals((float)i/(float)Population_size);
            std::array<MAX_INDIVIDUALS_MERGED, float> probas_array;
            Quality_Selection<MAX_INDIVIDUALS_MERGED, float> q;
            for(int j=0; j<MAX_INDIVIDUALS_MERGED; j++)
                probas_array[j] = probas[j];
            auto chosen_n_choices = q.apply(probas_array);
            n_choices = chosen_n_choices[0]+1;
        }
        else
        {
            if(rand()%10<4)
                n_choices = 1;
            else
                n_choices = 2;
        }

        if(n_choices+i > Population_size)
            n_choices = Population_size-i;

        if(choose_individuals_to_merge)
            choose_individuals_to_merge(n_choices, evaluated, cur_choice, nb_to_keep);
        else
            default_choose_individuals(n_choices, evaluated, cur_choice, nb_to_keep);

        if(n_choices==1)
            next_generation[i] = bits_of_individuals[sorted_selected[cur_choice[0]]];
        else
        {
            int min = cur_choice[0];
            for(int j=0; j<n_choices; j++)
            {
                next_generation[i+j] = bits_of_individuals[sorted_selected[cur_choice[j]]];
                if(cur_choice[j] < min)
                    min = cur_choice[j];
            }

            if(!proba_n_crossovers)
                n_cuts = 1;
            else
            {
                auto probas = proba_n_crossovers(min);
                Quality_Selection<Max_nb_crossovers, float> q;
                auto chosen_cuts = q.apply(probas);
                n_cuts = chosen_cuts[0]+1;
            }
            if(n_choices==2)
                cross(next_generation[i], next_generation[i+1], genes, nb_cuts);
            else
            {
                std::vector<std::vector<char>&> tmp;
                for(int j=0; j<n_choices; j++)
                    tmp.emplace_back(next_generation[j]);
                cross_multiple(tmp, genes, nb_cuts);
            }

        }
        i += n_choices;
    }

    std::swap(bits_of_individuals, next_generation);
}

void Genetic_Algorithm::hook_data()
{
    hook_object.new_step();
    hook_object.set(bits_of_individuals);
    hook_object.set(evaluated);
}


#endif
