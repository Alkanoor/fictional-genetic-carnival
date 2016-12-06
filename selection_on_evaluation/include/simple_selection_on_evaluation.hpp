#ifndef SIMPLE_SELECTION_ON_EVALUATION_HPP
#define SIMPLE_SELECTION_ON_EVALUATION_HPP


#define SIMPLE_SELECTION_THREAD_DEBUG


#include <functional>
#include <stdexcept>
#include <memory>

#include "utils/vector_to_string_force_type.hpp"
#include "selection/include/selection.hpp"
#include "utils/vector_to_string.hpp"


///*********************************************************************************
/// Class that provides basic composition of evaluation and selection on the result
///*********************************************************************************

template <size_t Population_size, typename T, size_t N_threads=1>
class Simple_Selection_On_Evaluation : public Selection_On_Evaluation<Population_size, std::array<int, Population_size> >
{
    public:
        Simple_Selection_On_Evaluation(const std::function<T(const std::vector<char>&, Genotype&)>& eval, const std::shared_ptr<Selection<Population_size,T,N_threads> >& select);

        void set_evaluation_selection(const std::function<T(const std::vector<char>&, Genotype&)>& eval, const std::shared_ptr<Selection<Population_size,T,N_threads> >& select);

        const std::array<T, Population_size>& eval(const std::array<std::vector<char>, Population_size>&, Genotype&) throw();
        const std::array<int, Population_size>& eval_select(const std::array<std::vector<char>, Population_size>&, Genotype&) throw();

    private:
        std::function<T(const std::vector<char>&, Genotype&)> evaluation;
        std::shared_ptr<Selection<Population_size,T,N_threads> > selection;

        std::array<T, Population_size> qualities;
};


template <size_t Population_size, typename T, size_t N_threads>
Simple_Selection_On_Evaluation<Population_size, T, N_threads>::Simple_Selection_On_Evaluation(const std::function<T(const std::vector<char>&, Genotype&)>& eval, const std::shared_ptr<Selection<Population_size,T,N_threads> >& select) :
    evaluation(eval),
    selection(select)
{}

template <size_t Population_size, typename T, size_t N_threads>
void Simple_Selection_On_Evaluation<Population_size, T, N_threads>::set_evaluation_selection(const std::function<T(const std::vector<char>&, Genotype&)>& eval, const std::shared_ptr<Selection<Population_size,T,N_threads> >& select)
{
    evaluation = eval;
    selection = select;
}

template <size_t Population_size, typename T, size_t N_threads>
const std::array<T, Population_size>& Simple_Selection_On_Evaluation<Population_size, T, N_threads>::eval(const std::array<std::vector<char>, Population_size>& bits, Genotype& genes) throw()
{
    if(selection)
    {
        #ifdef SIMPLE_SELECTION_THREAD_DEBUG
            auto logger = Easy_Log_In_File_Threaded_Debug::getInstance().getInfoLog();
        #endif

        for(int i=0; i<(int)Population_size; i++)
        {
            #ifdef SIMPLE_SELECTION_THREAD_DEBUG
                logger->write("In simple selection applying evaluation on ", Vector_To_String_Force_Type<int, std::vector<char> >(&bits[i]));
            #endif
            qualities[i] = evaluation(bits[i], genes);
        }

        return qualities;
    }
    else
        throw std::runtime_error("Error: bad selection pointer in simple selection on evaluation");
}

template <size_t Population_size, typename T, size_t N_threads>
const std::array<int, Population_size>& Simple_Selection_On_Evaluation<Population_size, T, N_threads>::eval_select(const std::array<std::vector<char>, Population_size>& bits, Genotype& genes) throw()
{
    if(selection)
    {
        eval(bits, genes);

        #ifdef SIMPLE_SELECTION_THREAD_DEBUG
            auto logger = Easy_Log_In_File_Threaded_Debug::getInstance().getInfoLog();
            logger->write("In simple selection applying selection on ", Vector_To_String<std::array<T, Population_size> >(&qualities));
        #endif

        return selection->apply(qualities);
    }
    else
        throw std::runtime_error("Error: bad selection pointer in simple selection on evaluation");
}


#endif
