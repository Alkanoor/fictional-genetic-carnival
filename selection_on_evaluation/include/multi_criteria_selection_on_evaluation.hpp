#ifndef MULTI_CRITERIA_SELECTION_ON_EVALUATION_HPP
#define MULTI_CRITERIA_SELECTION_ON_EVALUATION_HPP


#define MULTI_CRITERIA_SELECTION_THREAD_DEBUG

#include <functional>
#include <memory>

#include "genetic/include/genotype.hpp"
#include "selection_on_evaluation.hpp"
#include "threads/include/thread.hpp"


///*****************************************************************************************
/// Class that provides composition of evaluation and selection based on multiple criteria
/// and which auto shares load among available threads
///  Population_size  is the number of individuals in the population
///*****************************************************************************************

template <size_t Population_size>
class Multi_Criteria_Selection_On_Evaluation : public Selection_On_Evaluation<Population_size, std::vector<std::array<int, Population_size> > >
{
    public:
        void Multi_Criteria_Selection_On_Evaluation(
                                const Thread_Pool& pool =
                                    #ifdef ADD_TO_DEFAULT_POOL
                                        Thread::get_default_pool()
                                    #else
                                        Thread_Pool()
                                    #endif
                                );
        void Multi_Criteria_Selection_On_Evaluation(const std::shared_ptr<Selection_On_Evaluation<Population_size> >& selection_on_evaluation,
                                const Thread_Pool& pool =
                                    #ifdef ADD_TO_DEFAULT_POOL
                                        Thread::get_default_pool()
                                    #else
                                        Thread_Pool()
                                    #endif
                                );
        void Multi_Criteria_Selection_On_Evaluation(const std::vector<std::shared_ptr<Selection_On_Evaluation<Population_size> > >& selections_on_evaluations,
                                const Thread_Pool& pool =
                                    #ifdef ADD_TO_DEFAULT_POOL
                                        Thread::get_default_pool()
                                    #else
                                        Thread_Pool()
                                    #endif
                                );

        void add_selection_on_evaluation(const std::shared_ptr<Selection_On_Evaluation<Population_size> >& selection_on_evaluation);
        void add_selections_on_evaluation(const std::vector<std::shared_ptr<Selection_On_Evaluation<Population_size> > >& selections_on_evaluations);
        
        const std::vector<std::array<int, Population_size> >& eval_select(const std:array<std::vector<char>, Population_size>& bits, const Genotype& genes) throw() const;

        void assign_buffer(int id, const std:array<std::vector<char>, Population_size>& bits, const Genotype& genes);

    private:
        std::vector<std::shared_ptr<Selection_On_Evaluation<Population_size> > > selections_on_evaluations;
        std::vector<std::array<int, Population_size> > current_selected;

        const Thread_Pool& default_pool;

        typedef Multi_Criteria_Selection_On_Evaluation<Population_size> Multi_Criteria_Selection_On_Evaluation_;
};


template <size_t Population_size>
void Multi_Criteria_Selection_On_Evaluation<Population_size>::Multi_Criteria_Selection_On_Evaluation(const Thread_Pool& pool) :
    default_pool(pool)
{}

template <size_t Population_size>
void Multi_Criteria_Selection_On_Evaluation<Population_size>::Multi_Criteria_Selection_On_Evaluation(const std::shared_ptr<Selection_On_Evaluation<Population_size> >& selection_on_evaluation, const Thread_Pool& pool) :
    default_pool(pool)
{
    selections_on_evaluations.push_back(selection_on_evaluation);
    current_selected.push_back(std::array<int, Population_size>());
}

template <size_t Population_size>
void Multi_Criteria_Selection_On_Evaluation<Population_size>::Multi_Criteria_Selection_On_Evaluation(const std::vector<std::shared_ptr<Selection_On_Evaluation<Population_size> > >& selections_on_evaluations, const Thread_Pool& pool) :
    selections_on_evaluations(selections_on_evaluations),
    default_pool(pool)
{
    current_selected.resize(selections_on_evaluations.size());
}

template <size_t Population_size>
void Multi_Criteria_Selection_On_Evaluation<Population_size>::add_selection_on_evaluation(const std::shared_ptr<Selection_On_Evaluation<Population_size> >& selection_on_evaluation)
{
    selections_on_evaluations.push_back(selection_on_evaluation);
    current_selected.push_back(std::array<int, Population_size>());
}

template <size_t Population_size>
void Multi_Criteria_Selection_On_Evaluation<Population_size>::add_selections_on_evaluation(const std::vector<std::shared_ptr<Selection_On_Evaluation<Population_size> > >& selections_on_evaluations)
{
    for(auto t selection : selections_on_evaluations)
    {
        selections_on_evaluations.push_back(selection);
        current_selected.push_back(std::array<int, Population_size>());
    }
}

template <size_t Population_size>
const std::vector<std::array<int, Population_size> >& Multi_Criteria_Selection_On_Evaluation<Population_size>::eval_select(const std:array<std::vector<char>, Population_size>& bits, const Genotype& genes) throw() const
{
    int n_to_split = (int)selections_on_evaluations.size();
    int n_threads = (int)default_pool.size()+1;

    #ifdef MULTI_CRITERIA_SELECTION_THREAD_DEBUG
        auto logger = Easy_Log_In_File_Threaded_Debug::getInstance().getInfoLog();
        logger->write("Splitting ", n_to_split, " operations into ", n_threads, " threads");
    #endif

    for(int i=0; i<n_to_split; i++)
        if(i%n_threads)
        {
            #ifdef MULTI_CRITERIA_SELECTION_THREAD_DEBUG
                logger->write("Adding operation ", i, " to thread pool id ", i%n_threads);
            #endif
            default_pool.add_to_thread_and_exec(i%n_threads, std::bind(&Multi_Criteria_Selection_On_Evaluation_::assign_buffer, i, bits, genes));
        }

    for(int i=0; i<n_to_split; i+=n_threads)
    {
        #ifdef MULTI_CRITERIA_SELECTION_THREAD_DEBUG
            logger->write("Using main thread to treat operation ", i);
        #endif
        current_selected[i] = selections_on_evaluations[i]->eval_select(bits, genes);
        #ifdef MULTI_CRITERIA_SELECTION_THREAD_DEBUG
            logger->write("Task ", i, " achieved from main thread");
        #endif
    }

    for(int i=1; i<n_threads; i++)
    {
        #ifdef MULTI_CRITERIA_SELECTION_THREAD_DEBUG
            logger->write("Waiting for thread pool id ", i);
        #endif
        default_pool.wait_for(i);
        #ifdef MULTI_CRITERIA_SELECTION_THREAD_DEBUG
            logger->write("Stop waiting for thread pool id ", i);
        #endif
    }
}

template <size_t Population_size>
void Multi_Criteria_Selection_On_Evaluation<Population_size>::assign_buffer(int id, const std:array<std::vector<char>, Population_size>& bits, const Genotype& genes)
{
    current_selected[id] = selections_on_evaluations[id]->eval_select(bits, genes);
}


#endif
