#ifndef MULTI_CRITERIA_SELECTION_ON_EVALUATION_HPP
#define MULTI_CRITERIA_SELECTION_ON_EVALUATION_HPP


#include <bitset>
#include <memory>
#include <functional>

#include "threads/include/thread.hpp"
#include "selection_on_evaluation.hpp"


///*****************************************************************************************
/// Class that provides composition of evaluation and selection based on multiple criteria
/// and which auto shares load among available threads
///*****************************************************************************************

template <size_t Population_size, size_t N_bits>
class Multi_Criteria_Selection_On_Evaluation : public Selection_On_Evaluation<Population_size, N_bits, std::vector<std::array<int, Population_size> > >
{
    public:
        void add_selection_on_evaluation(const std::shared_ptr<Selection_On_Evaluation<Population_size, N_bits> >& selection_on_evaluation,
                                const Thread_Pool& pool =
                                    #ifdef ADD_TO_DEFAULT_POOL
                                        Thread::get_default_pool()
                                    #else
                                        Thread_Pool()
                                    #endif
                                );
        void add_selection_on_evaluation(const std::vector<std::shared_ptr<Selection_On_Evaluation<Population_size, N_bits> > >& selections_on_evaluations,
                                const Thread_Pool& pool =
                                    #ifdef ADD_TO_DEFAULT_POOL
                                        Thread::get_default_pool()
                                    #else
                                        Thread_Pool()
                                    #endif
                                );

        const std::vector<std::array<int, Population_size> >& eval_select(const std::bitset<N_bits>&, const Genotype&) throw() const;

        void assign_buffer(int id, const std::bitset<N_bits>& bits, const Genotype& genes);

    private:
        std::vector<std::shared_ptr<Selection_On_Evaluation<Population_size, N_bits> > > selections_on_evaluations;
        std::vector<std::array<int, Population_size> > current_selected;

        const Thread_Pool& default_pool;

        typedef Multi_Criteria_Selection_On_Evaluation<Population_size, N_bits> Multi_Criteria_Selection_On_Evaluation_;
};


template <size_t Population_size, size_t N_bits>
void Multi_Criteria_Selection_On_Evaluation<Population_size, N_bits>::add_selection_on_evaluation(const std::shared_ptr<Selection_On_Evaluation<Population_size, N_bits> >& selection_on_evaluation, const Thread_Pool& pool) :
    default_pool(pool)
{
    selections_on_evaluations.push_back(selection_on_evaluation);
    current_selected.push_back(std::array<int, Population_size>());
}

template <size_t Population_size, size_t N_bits>
void Multi_Criteria_Selection_On_Evaluation<Population_size, N_bits>::add_selection_on_evaluation(const std::vector<std::shared_ptr<Selection_On_Evaluation<Population_size, N_bits> > >& selections_on_evaluations, const Thread_Pool& pool) :
    selections_on_evaluations(selections_on_evaluations),
    default_pool(pool)
{
    current_selected.resize(selections_on_evaluations.size());
}

template <size_t Population_size, size_t N_bits>
const std::vector<std::array<int, Population_size> >& Multi_Criteria_Selection_On_Evaluation<Population_size, N_bits>::eval_select(const std::bitset<N_bits>& bits, const Genotype& genes) throw() const
{
    int n_to_split = (int)selections_on_evaluations.size();
    int n_threads = (int)default_pool.size()+1;

    for(int i=0; i<n_to_split; i++)
        if(i%n_threads)
            default_pool.add_to_thread_and_exec(i%n_threads, std::bind(&Multi_Criteria_Selection_On_Evaluation_::assign_buffer, i, bits, genes));

    for(int i=0; i<n_to_split; i+=n_threads)
        current_selected[i] = selections_on_evaluations[i]->eval_select(bits, genes);

    for(int i=0; i<n_to_split; i++)
        if(i%n_threads)
            default_pool.wait_for(i%n_threads);
}

template <size_t Population_size, size_t N_bits>
void Multi_Criteria_Selection_On_Evaluation<Population_size, N_bits>::assign_buffer(int id, const std::bitset<N_bits>& bits, const Genotype& genes)
{
    current_selected[id] = selections_on_evaluations[id]->eval_select(bits, genes);
}


#endif
