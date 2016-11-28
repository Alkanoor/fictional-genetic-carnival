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
        void add_gene(const Gene<T>& g);

        void interprete(const std::vector<char>& adn);

        const std::vector<int>& get_integer_interpreted() const;
        const std::vector<float>& get_float_interpreted() const;

    private:
        std::vector<Gene<int> > integer_genes;
        std::vector<Gene<float> > float_genes;
        std::vector<char> current_gene;
};


template <>
void add_gene<float>(const std::string& name, int min_location_on_adn, int max_location_on_adn, const std::function<float(const std::vector<char>&, int, int)>& interpretation)
{
    assert(min_location_on_adn >= 0 && max_location_on_adn > min_location_on_adn);

    names_float_interpreted.push_back(name);
    float_interpreted.push_back(0);
    min_location_float_genes.push_back(min_location_on_adn);
    max_location_float_genes.push_back(max_location_on_adn);
    float_interpretations.push_back(interpretation);

    if(max_location_on_adn > max_location)
        max_location = max_location_on_adn;
}


#endif
