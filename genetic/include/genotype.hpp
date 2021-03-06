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
        Genotype(bool save_current_adn = false);

        void add_gene(const Gene<int>& g);
        void add_gene(const Gene<float>& g);

        void interprete(const std::vector<char>& adn);

        const std::vector<int>& get_integer_interpreted() const;
        const std::vector<float>& get_float_interpreted() const;
        const std::vector<char>& get_current_gene() const;

        const Gene<int>& get_gene_int(int id) const;
        const Gene<float>& get_gene_float(int id) const;
        const Gene<int>& get_gene_int(const std::string& s) const;
        const Gene<float>& get_gene_float(const std::string& s) const;

        size_t get_number_integer_genes() const;
        size_t get_number_float_genes() const;
        int get_min_location() const;
        int get_max_location() const;

    private:
        bool save_current_adn;

        std::vector<int> integer_interpreted;
        std::vector<float> float_interpreted;

        std::vector<Gene<int> > integer_genes;
        std::vector<Gene<float> > float_genes;
        std::vector<char> current_gene;

        int min_adn_location;
        int max_adn_location;
};


#endif
