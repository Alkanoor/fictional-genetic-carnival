#ifndef SELECTION_ON_EVALUATION_HPP
#define SELECTION_ON_EVALUATION_HPP


#include <bitset>

#include "genetics/include/genotype.hpp"


///*********************************************************************************
/// Abstract class that provides template for selections on evaluations
///*********************************************************************************

template <size_t Population_size, size_t N_bits, typename T>
class Simple_Selection_On_Evaluation
{
    public:
        virtual const T& eval_select(const std::bitset<N_bits>&, const Genotype&) throw() const = 0;
};


#endif
