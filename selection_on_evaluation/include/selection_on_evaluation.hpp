#ifndef SELECTION_ON_EVALUATION_HPP
#define SELECTION_ON_EVALUATION_HPP


#include <vector>

#include "genetics/include/genotype.hpp"


///*********************************************************************************
/// Abstract class that provides template for selections on evaluations
///*********************************************************************************

template <size_t Population_size, typename T>
class Simple_Selection_On_Evaluation
{
    public:
        virtual const T& eval_select(const std::array<std::vector<char>, Population_size>&, const Genotype&) throw() const = 0;
};


#endif
