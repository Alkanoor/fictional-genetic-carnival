#ifndef SELECTION_ON_EVALUATION_HPP
#define SELECTION_ON_EVALUATION_HPP


#include <vector>

#include "genetic/include/genotype.hpp"


///*********************************************************************************
/// Abstract class that provides template for selections on evaluations
///*********************************************************************************

template <size_t Population_size, typename T, typename U>
class Selection_On_Evaluation
{
    public:
        virtual const std::array<T, Population_size>& eval(const std::array<std::vector<char>, Population_size>&, Genotype&) throw() = 0;
        virtual const U& eval_select(const std::array<std::vector<char>, Population_size>&, Genotype&) throw() = 0;
};


#endif
