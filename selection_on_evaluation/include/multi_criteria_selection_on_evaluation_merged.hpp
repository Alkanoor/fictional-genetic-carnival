#ifndef MULTI_CRITERIA_SELECTION_ON_EVALUATION_MERGED_HPP
#define MULTI_CRITERIA_SELECTION_ON_EVALUATION_MERGED_HPP


#include "multi_criteria_selection_on_evaluation.hpp"


///*****************************************************************************************
/// Class that provides merging of previously separated and processed data in a
/// multi_criteria_selection_on_evaluation instance
///*****************************************************************************************

template <size_t Population_size, typename T>
class Multi_Criteria_Selection_On_Evaluation_Merged : public Selection_On_Evaluation_With_Evaluation<Population_size, T, std::array<int, Population_size> >
{
    public:
        const std::array<T, Population_size>& eval(const std::array<std::vector<char>, Population_size>&, Genotype&) throw();
        const std::array<int, Population_size>& eval_select(const std::array<std::vector<char>, Population_size>&, Genotype&) throw();

    private:
        std::shared_ptr<Multi_Criteria_Selection_On_Evaluation<Population_size, T> > wrapped;
};


#endif
