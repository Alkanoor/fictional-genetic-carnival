#ifndef GENOTYPE_HPP
#define GENOTYPE_HPP


#include <bitset>
#include <memory>
#include <stdexcept>
#include <functional>

#include "selection/include/selection.hpp"


///***********************************************************************************
/// Class that provides genotype model : association of bit range and interpretation
///***********************************************************************************

class Genotype
{
    public:
        Genotype();

        // T is either int or float
        // interpretation could be anything : Gray code, binary conversion, ...
        template <typename T>
        void add_gene(const std::string& name, int min_location_on_adn, int max_location_on_adn, const std::function<T(const std::vector<char>&, int, int)>& interpretation);

        template <size_t N>
        void interprete(const std::vector<char>& adn);

        const std::vector<int>& get_integer_interpreted() const;
        const std::vector<float>& get_integer_interpreted() const;

    private:
        std::vector<int> integer_interpreted;
        std::vector<float> float_interpreted;

        std::vector<std::function<int(const std::vector<char>&, int, int)> > integer_interpretations;
        std::vector<std::function<float(const std::vector<char>&, int, int)> > float_interpretations;

        std::vector<std::string> names_integer_interpreted;
        std::vector<std::string> names_float_interpreted;

        int max_bits_in_range;
        std::vector<char> current_gene;
};


template <size_t Population_size, typename T, size_t N_bits, size_t N_threads>
Simple_Selection_On_Evaluation<Population_size, T, N_bits, N_threads>::Simple_Selection_On_Evaluation(const std::function<const std::array<T, Population_size>&(const std::vector<char>&, const Genotype&)>& eval, const std::shared_ptr<Selection<Population_size,T,N_threads> >& select) :
    evaluation(eval),
    selection(select)
{}

template <size_t Population_size, typename T, size_t N_bits, size_t N_threads>
void Simple_Selection_On_Evaluation<Population_size, T, N_bits, N_threads>::set_evaluation_selection(const std::function<const std::array<T, Population_size>&(const std::vector<char>&, const Genotype&)>& eval, const std::shared_ptr<Selection<Population_size,T,N_threads> >& select)
{
    evaluation = eval;
    selection = select;
}

template <size_t Population_size, typename T, size_t N_bits, size_t N_threads>
const std::array<int, Population_size>& Simple_Selection_On_Evaluation<Population_size, T, N_bits, N_threads>::eval_select(const std::vector<char>& bits, const Genotype& genes) throw() const
{
    if(selection)
        return selection->apply(evaluation(bits, genes));
    else
        throw std::runtime_error("Error: bad selection pointer in simple selection on evaluation");
}


#endif
