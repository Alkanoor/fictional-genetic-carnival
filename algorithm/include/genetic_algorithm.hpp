#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP


#define MAX_INDIVIDUALS_MERGED 10
#define MAX_N_CROSSOVERS 10
#define LOG_STEPS


#include "genetic_utils.hpp"
#include "hook_object.hpp"

#include <set>


///************************************************************************************************************
/// Class that provides core of genetic algorithm
///
/// - proba_n_crossovers : at index i is probability of having i+1 cut(s) in ADN during crossover
///                        (depend of minimum rank of current considered individuals)
///                        (if not set, default is proba 1 for 2 cuts in ADN)
/// - proba_merging_n_individuals : at index i is probability of merging i+1 individuals during crossover
///                                 (depend of ratio of processed individuals)
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

template <size_t Population_size, typename T=float, size_t Max_nb_crossovers=10>
class Genetic_Algorithm
{
    public:
        Genetic_Algorithm(size_t N_iterations, size_t to_keep, float mutation_rate, const Genotype& genes,
                          const std::shared_ptr<Selection_On_Evaluation<Population_size, T, std::array<int, Population_size> > >& eval_select,
                          const std::function<void(const Genotype&, std::vector<char>&)>& generate = std::bind(&Genetic_Utils::static_basic_random_individual_0_1, std::placeholders::_1, std::placeholders::_2),
                          const std::function<void(std::vector<char>&, const Genotype&, float)>& mutate = std::bind(&Genetic_Utils::mutate_basic_uniform_flip, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                          const std::function<void(std::vector<char>&, std::vector<char>&, const Genotype&, int)>& cross = std::bind(&Genetic_Utils::cross_basic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)
                          #ifdef LOG_STEPS
                             , const std::shared_ptr<Info_Warning_Error_Logger_Threaded>& log = Easy_Log_In_File_Threaded::getInfoLog()
                          #endif
                         );

        void generate_population();
        void evaluate_and_select();
        void mutate_and_cross_over();

        void run();

        const Genotype& get_genes() const;
        const std::array<T, Population_size>& get_evaluated() const;
        const std::array<std::vector<char>, Population_size>& get_population() const;

        void set_hook_object(const std::shared_ptr<Hook_Object<T, Population_size> >& hook);

        static void default_choose_individuals(int n_choices, std::array<int, Population_size>& cur_choice, int nb_to_keep);

    private:
        size_t N_iterations;
        size_t nb_to_keep;

        std::function<void(const Genotype&, std::vector<char>&)> random_individual;
        std::shared_ptr<Selection_On_Evaluation<Population_size, T, std::array<int, Population_size> > > eval_and_select;

        std::function<void(std::vector<char>&, const Genotype&, float)> mutate;
        std::function<void(std::vector<char>&, std::vector<char>&, const Genotype&, int)> cross;
        std::function<void(std::vector<std::vector<char>*>&, const Genotype&, int)> cross_multiple;

        std::function<const std::vector<float>&(float)> proba_merging_n_individuals;
        std::function<const std::array<float, Max_nb_crossovers>&(int)> proba_n_crossovers;

        std::function<void(int, const std::array<T, Population_size>&, std::array<int, Population_size>&, int)> choose_individuals_to_merge;

        Genotype genes;
        std::array<T, Population_size> evaluated;
        std::array<int, Population_size> sorted_selected;
        std::array<std::vector<char>, Population_size> bits_of_individuals;
        std::array<std::vector<char>, Population_size> next_generation;

        float mutation_rate;

        #ifdef LOG_STEPS
            std::shared_ptr<Info_Warning_Error_Logger> logger;
        #endif

        std::shared_ptr<Hook_Object<T, Population_size> > hook_object;
        void hook_data();
};


template <size_t Population_size, typename T, size_t Max_nb_crossovers>
Genetic_Algorithm<Population_size, T, Max_nb_crossovers>::Genetic_Algorithm(size_t N_iterations, size_t to_keep, float mutation_rate, const Genotype& genes,
                  const std::shared_ptr<Selection_On_Evaluation<Population_size, T, std::array<int, Population_size> > >& eval_select,
                  const std::function<void(const Genotype&, std::vector<char>&)>& generate,
                  const std::function<void(std::vector<char>&, const Genotype&, float)>& mutate,
                  const std::function<void(std::vector<char>&, std::vector<char>&, const Genotype&, int)>& cross
                  #ifdef LOG_STEPS
                    ,const std::shared_ptr<Info_Warning_Error_Logger_Threaded>& log
                  #endif
              ) :
    N_iterations(N_iterations),
    nb_to_keep(to_keep),
    random_individual(generate),
    eval_and_select(eval_select),
    mutate(mutate),
    cross(cross),
    genes(genes),
    mutation_rate(mutation_rate)
    #ifdef LOG_STEPS
        ,logger(log)
    #endif
{
    assert(to_keep>2);
    assert(Population_size>=to_keep);
}

template <size_t Population_size, typename T, size_t Max_nb_crossovers>
void Genetic_Algorithm<Population_size, T, Max_nb_crossovers>::generate_population()
{
    #ifdef LOG_STEPS
        logger->write("[+] Generating ", Population_size, " sized population ");
    #endif
    for(int i=0; i<(int)Population_size; i++)
        random_individual(genes, bits_of_individuals[i]);
}

template <size_t Population_size, typename T, size_t Max_nb_crossovers>
void Genetic_Algorithm<Population_size, T, Max_nb_crossovers>::evaluate_and_select()
{
    #ifdef LOG_STEPS
        logger->write("[+] Evaluating and selecting current population");
    #endif

    sorted_selected = eval_and_select->eval_select(bits_of_individuals, genes);

    #ifdef LOG_STEPS
        logger->write("[.] Current selection order is ", Vector_To_String<std::array<int, Population_size> >(&sorted_selected));
    #endif

    if(hook_object)
        evaluated = eval_and_select->eval(bits_of_individuals, genes);
}

template <size_t Population_size, typename T, size_t Max_nb_crossovers>
void Genetic_Algorithm<Population_size, T, Max_nb_crossovers>::mutate_and_cross_over()
{
    #ifdef LOG_STEPS
        logger->write("[+] Crossing over on current population");
        logger->write("[.] Keeping ", nb_to_keep, " individuals out of ", Population_size, " :");
        for(int i=0; i<(int)nb_to_keep; i++)
            (*logger)<<sorted_selected[i]<<" ";
        logger->endLine();
    #endif

    //first individuals are selected ones
    for(int i=0; i<(int)nb_to_keep; i++)
        next_generation[i] = bits_of_individuals[sorted_selected[i]];

    //crossover
    int n_cuts = 1, n_choices = 1;
    std::array<int, Population_size> cur_choice;
    for(int i=nb_to_keep; i<(int)Population_size;)
    {
        //choose number of individuals that will be merged
        if(cross_multiple && proba_merging_n_individuals)
        {
            auto probas = proba_merging_n_individuals((float)i/(float)Population_size);
            std::array<float, MAX_INDIVIDUALS_MERGED> probas_array;
            Quality_Selection<MAX_INDIVIDUALS_MERGED, float> q;
            for(int j=0; j<MAX_INDIVIDUALS_MERGED && j<(int)probas.size(); j++)
                probas_array[j] = probas[j];
            auto chosen_n_choices = q.apply(probas_array);
            n_choices = chosen_n_choices[0]+1;
        }
        else
        {
            //40% chance not beeing merged if no specific behaviour is set
            if(rand()%10<4)
                n_choices = 1;
            else
                n_choices = 2;
        }

        //we can't merge more individuals than possible
        if(n_choices+i > (int)Population_size)
            n_choices = Population_size-i;

        #ifdef LOG_STEPS
            logger->write("[.] ", n_choices, " individual(s) will be merged");
        #endif

        //choose individuals to merge, default is random different selected individuals
        if(choose_individuals_to_merge)
            choose_individuals_to_merge(n_choices, evaluated, cur_choice, nb_to_keep);
        else
            default_choose_individuals(n_choices, cur_choice, nb_to_keep);

        //perform the crossover(s) if there is
        if(n_choices==1)
        {
            #ifdef LOG_STEPS
                logger->write("[.] ", cur_choice[0], " individual id is repeated");
            #endif
            next_generation[i] = bits_of_individuals[sorted_selected[cur_choice[0]]];
        }
        else
        {
            (*logger)<<"[.] ";
            int min = cur_choice[0];
            for(int j=0; j<n_choices; j++)
            {
                (*logger)<<sorted_selected[cur_choice[j]]<<" ";
                next_generation[i+j] = bits_of_individuals[sorted_selected[cur_choice[j]]];
                if(cur_choice[j] < min)
                    min = cur_choice[j];
            }
            #ifdef LOG_STEPS
                (*logger)<<" will be merged";
                logger->endLine();
            #endif

            if(!proba_n_crossovers)
                n_cuts = 2;
            else
            {
                std::array<float, Max_nb_crossovers> probas = proba_n_crossovers(min);
                Quality_Selection<Max_nb_crossovers, float> q;
                auto chosen_cuts = q.apply(probas);
                n_cuts = chosen_cuts[0]+1;
            }
            #ifdef LOG_STEPS
                logger->write("[.] ", n_cuts, " cuts will be performed");
            #endif

            if(n_choices==2)
                cross(next_generation[i], next_generation[i+1], genes, n_cuts);
            else
            {
                std::vector<std::vector<char>*> tmp;
                for(int j=0; j<n_choices; j++)
                    tmp.emplace_back(&next_generation[j]);
                cross_multiple(tmp, genes, n_cuts);
            }

        }
        i += n_choices;
    }

    #ifdef LOG_STEPS
        logger->write("[+] Mutating population");
    #endif

    //mutate
    for(int i=nb_to_keep; i<(int)Population_size; i++)
        mutate(next_generation[i], genes, mutation_rate);

    //swap with normal population
    std::swap(bits_of_individuals, next_generation);
}

template <size_t Population_size, typename T, size_t Max_nb_crossovers>
void Genetic_Algorithm<Population_size, T, Max_nb_crossovers>::run()
{
    generate_population();
    evaluated = eval_and_select->eval(bits_of_individuals, genes);

    #ifdef LOG_STEPS
        logger->write("[+] At the beginning evaluation should be bad : ", Vector_To_String<std::array<T,Population_size> >(&evaluated));
    #endif

    for(int i=0; i<(int)N_iterations;i++)
    {
        #ifdef LOG_STEPS
            logger->write("[+] Performing iteration ", i);
        #endif
        evaluate_and_select();
        if(hook_object)
            hook_data();
        mutate_and_cross_over();
    }

    evaluate_and_select();
    if(hook_object)
        hook_data();
    else
    {
        evaluated = eval_and_select->eval(bits_of_individuals, genes);
        #ifdef LOG_STEPS
            logger->write("[+] At the end evaluation should be good : ", Vector_To_String<std::array<T,Population_size> >(&evaluated));
        #endif
    }
}

template <size_t Population_size, typename T, size_t Max_nb_crossovers>
const Genotype& Genetic_Algorithm<Population_size, T, Max_nb_crossovers>::get_genes() const
{return genes;}

template <size_t Population_size, typename T, size_t Max_nb_crossovers>
const std::array<T, Population_size>& Genetic_Algorithm<Population_size, T, Max_nb_crossovers>::get_evaluated() const
{return evaluated;}

template <size_t Population_size, typename T, size_t Max_nb_crossovers>
const std::array<std::vector<char>, Population_size>& Genetic_Algorithm<Population_size, T, Max_nb_crossovers>::get_population() const
{return bits_of_individuals;}

template <size_t Population_size, typename T, size_t Max_nb_crossovers>
void Genetic_Algorithm<Population_size, T, Max_nb_crossovers>::set_hook_object(const std::shared_ptr<Hook_Object<T, Population_size> >& hook)
{
    hook_object = hook;
}

template <size_t Population_size, typename T, size_t Max_nb_crossovers>
void Genetic_Algorithm<Population_size, T, Max_nb_crossovers>::default_choose_individuals(int n_choices, std::array<int, Population_size>& cur_choice, int nb_to_keep)
{
    int tmp;
    std::set<int> counter;
    for(int i=0; i<n_choices; i++)
    {
        while(counter.count(tmp=(rand()%nb_to_keep)))
            ;
        counter.insert(tmp);
        cur_choice[i] = tmp;
    }
}

template <size_t Population_size, typename T, size_t Max_nb_crossovers>
void Genetic_Algorithm<Population_size, T, Max_nb_crossovers>::hook_data()
{
    hook_object->new_step();
    hook_object->set(bits_of_individuals);
    hook_object->set(evaluated);
}


#endif
