#ifndef SIMPLE_SELECTION_ON_EVALUATION_HPP
#define SIMPLE_SELECTION_ON_EVALUATION_HPP


#include <memory>
#include <stdexcept>
#include <functional>

#include "selection/include/selection.hpp"


///*********************************************************************************
/// Class that provides basic composition of evaluation and selection on the result
///*********************************************************************************

template <size_t Population_size, typename T, size_t N_threads=1>
class Simple_Selection_On_Evaluation : public Selection_On_Evaluation<Population_size, std::array<int, Population_size> >
{
    public:
        Simple_Selection_On_Evaluation(const std::function<const std::array<T, Population_size>&(const std::vector<char>&, const Genotype&)>& eval, const std::shared_ptr<Selection<Population_size,T,N_threads> >& select);

        void set_evaluation_selection(const std::function<const std::array<T, Population_size>&(const std::vector<char>&, const Genotype&)>& eval, const std::shared_ptr<Selection<Population_size,T,N_threads> >& select);

        const std::array<int, Population_size>& eval_select(const std::vector<char>&, const Genotype&) throw() const;

    private:
        std::function<const std::array<T, Population_size>&(const std::vector<char>&, const Genotype&)> evaluation;
        std::shared_ptr<Selection<Population_size,T,N_threads> > selection;
};


template <size_t Population_size, typename T, size_t N_threads>
Simple_Selection_On_Evaluation<Population_size, T, N_threads>::Simple_Selection_On_Evaluation(const std::function<const std::array<T, Population_size>&(const std::vector<char>&, const Genotype&)>& eval, const std::shared_ptr<Selection<Population_size,T,N_threads> >& select) :
    evaluation(eval),
    selection(select)
{}

template <size_t Population_size, typename T, size_t N_threads>
void Simple_Selection_On_Evaluation<Population_size, T, N_threads>::set_evaluation_selection(const std::function<const std::array<T, Population_size>&(const std::vector<char>&, const Genotype&)>& eval, const std::shared_ptr<Selection<Population_size,T,N_threads> >& select)
{
    evaluation = eval;
    selection = select;
}

template <size_t Population_size, typename T, size_t N_threads>
const std::array<int, Population_size>& Simple_Selection_On_Evaluation<Population_size, T, N_threads>::eval_select(const std::vector<char>& bits, const Genotype& genes) throw() const
{
    if(selection)
        return selection->apply(evaluation(bits, genes));
    else
        throw std::runtime_error("Error: bad selection pointer in simple selection on evaluation");
}


#endif
