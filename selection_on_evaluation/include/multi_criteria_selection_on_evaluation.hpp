#ifndef MULTI_CRITERIA_SELECTION_ON_EVALUATION_HPP
#define MULTI_CRITERIA_SELECTION_ON_EVALUATION_HPP


#include <bitset>
#include <memory>
#include <functional>

#include "selection_on_evaluation.hpp"


///*****************************************************************************************
/// Class that provides composition of evaluation and selection based on multiple criterion
///*****************************************************************************************

template <size_t Population_size, size_t N_bits, size_t N_threads=1>
class Multi_Criteria_Selection_On_Evaluation : public Selection_On_Evaluation<Population_size, N_bits, std::vector<std::array<int, Population_size> > >
{
    public:
        void add_selection_on_evaluation(const std::shared_ptr<Selection_On_Evaluation<Population_size, N_bits> >&);

        const std::vector<std::array<int, Population_size> >& eval_select(const std::bitset<N_bits>&, const Genotype&) throw() const;

    private:
        std::vector<std::shared_ptr<Selection_On_Evaluation<Population_size, N_bits> > > selections_on_evaluations;

        std::vector<std::array<int, Population_size> > current_selected;
};


template <size_t Population_size, size_t N_bits, size_t N_threads>
void Multi_Criteria_Selection_On_Evaluation<Population_size, N_bits, N_threads>::add_selection_on_evaluation(const std::shared_ptr<Selection_On_Evaluation<Population_size, N_bits> >& selection_on_evaluation)
{
    selections_on_evaluations.push_back(selection_on_evaluation);
    current_selected.push_back(std::array<int, Population_size>());
}

template <size_t Population_size, size_t N_bits, size_t N_threads>
const std::vector<std::array<int, Population_size> >& Multi_Criteria_Selection_On_Evaluation<Population_size, N_bits, N_threads>::eval_select(const std::bitset<N_bits>&, const Genotype&) throw() const
{
    assert(selections_on_evaluations>0);

    int ratio = (int)selections_on_evaluations.size()/(int)N_threads;
    int to_share = (int)N_threads-ratio;

    for(int i=0; i<to_share; i+=ratio)

    for(int i=0; i<ratio; i++)
        current_selected[i] = selections_on_evaluations[0]->
}


#endif