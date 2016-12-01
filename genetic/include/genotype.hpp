#ifndef GENOTYPE_HPP
#define GENOTYPE_HPP


#include <bitset>
#include <memory>
#include <stdexcept>
#include <functional>

#include "selection/include/selection.hpp"


///***********************************************************
/// Class that provides genotype model : array of genes
///***********************************************************

class Genotype
{
    public:
        Genotype();

        // T is either int or float
        template <typename T>
        void add_gene(const Gene<T>& g);

        void interprete(const std::vector<char>& adn);

        const std::vector<int>& get_integer_interpreted() const;
        const std::vector<float>& get_float_interpreted() const;
        const std::vector<char>& get_current_gene() const;

        // T is either int or float
        template <typename T>
        const Gene<T>& get_gene(int id) const;

    private:
        std::vector<int> integer_interpreted;
        std::vector<float> float_interpreted;

        std::vector<Gene<int> > integer_genes;
        std::vector<Gene<float> > float_genes;
        std::vector<char> current_gene;
};


template <>
void add_gene(const Gene<float>& g)
{
    float_genes.push_back(g);
    float_interpreted.push_back(0);
}

template <>
void add_gene(const Gene<int>& g)
{
    integer_genes.push_back(g);
    integer_interpreted.push_back(0);
}

template <>
const Gene<int>& get_gene(int id) const
{
    assert(id < (int)integer_genes.size());
    return integer_genes[id];
}

template <>
const Gene<float>& get_gene(int id) const
{
    assert(id < (int)float_genes.size());
    return float_genes[id];
}


#endif
