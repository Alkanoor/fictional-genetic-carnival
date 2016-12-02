#ifndef GENOTYPE_HPP
#define GENOTYPE_HPP


#include <functional>
#include <stdexcept>
#include <bitset>
#include <memory>

#include "selection/include/selection.hpp"
#include "gene.hpp"


///***********************************************************
/// Class that provides genotype model : array of genes
///***********************************************************

class Genotype
{
    public:
        void add_gene(const Gene<int>& g);
        void add_gene(const Gene<float>& g);

        void interprete(const std::vector<char>& adn);

        const std::vector<int>& get_integer_interpreted() const;
        const std::vector<float>& get_float_interpreted() const;
        const std::vector<char>& get_current_gene() const;

        const Gene<int>& get_gene_int(int id) const;
        const Gene<float>& get_gene_float(int id) const;

    private:
        std::vector<int> integer_interpreted;
        std::vector<float> float_interpreted;

        std::vector<Gene<int> > integer_genes;
        std::vector<Gene<float> > float_genes;
        std::vector<char> current_gene;
};


#endif
