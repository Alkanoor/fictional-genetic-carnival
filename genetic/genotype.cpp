#include "include/genotype.hpp"


void Genotype::add_gene(const Gene<float>& g)
{
    float_genes.push_back(g);
    float_interpreted.push_back(0);
}

void Genotype::add_gene(const Gene<int>& g)
{
    integer_genes.push_back(g);
    integer_interpreted.push_back(0);
}

void Genotype::interprete(const std::vector<char>& adn)
{
    current_gene = adn;

    for(int i=0; i<(int)integer_genes.size(); i++)
        integer_interpreted[i] = integer_genes[i].interprete(adn);
    for(int i=0; i<(int)float_genes.size(); i++)
        float_interpreted[i] = float_genes[i].interprete(adn);
}

const std::vector<int>& Genotype::get_integer_interpreted() const
{return integer_interpreted;}

const std::vector<float>& Genotype::get_float_interpreted() const
{return float_interpreted;}

const std::vector<char>& Genotype::get_current_gene() const
{return current_gene;}

const Gene<int>& Genotype::get_gene_int(int id) const
{
    assert(id < (int)integer_genes.size());
    return integer_genes[id];
}

const Gene<float>& Genotype::get_gene_float(int id) const
{
    assert(id < (int)float_genes.size());
    return float_genes[id];
}
