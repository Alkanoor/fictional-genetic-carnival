#ifndef SIMPLE_SELECTION_ON_EVALUATION_WITH_SHARING_HPP
#define SIMPLE_SELECTION_ON_EVALUATION_WITH_SHARING_HPP


#include "selection_on_evaluation.hpp"


///*******************************************************************************************
/// Class that provides a selection with sharing depending on supplied genotypic distance
/// Clustering is made of k-means++, parameters should be specified in constructor
///*******************************************************************************************

template <size_t Population_size, typename T>
class Selection_On_Evaluation_With_Sharing : public Selection_On_Evaluation<Population_size, T, std::array<int, Population_size> >
{
    public:
        const std::array<T, Population_size>& eval(const std::array<std::vector<char>, Population_size>&, Genotype&) throw();
        const std::array<int, Population_size>& eval_select(const std::array<std::vector<char>, Population_size>&, Genotype&) throw();

    private:
        std::shared_ptr<Selection_On_Evaluation<Population_size, T, std::array<int, Population_size> > wrapped;
};


#endif
