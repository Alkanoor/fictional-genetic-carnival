#include "include/genotype.hpp"

#include <stdexcept>


Genotype::Genotype(bool save_current_adn) :
    save_current_adn(save_current_adn),
    min_adn_location(0),
    max_adn_location(0)
{}

void Genotype::add_gene(const Gene<float>& g)
{
    float_genes.push_back(g);
    float_interpreted.push_back(0);
    if(g.get_min_location() < min_adn_location)
        min_adn_location = g.get_min_location();
    if(g.get_max_location() < max_adn_location)
        max_adn_location = g.get_max_location();
}

void Genotype::add_gene(const Gene<int>& g)
{
    integer_genes.push_back(g);
    integer_interpreted.push_back(0);
    if(g.get_min_location() < min_adn_location)
        min_adn_location = g.get_min_location();
    if(g.get_max_location() < max_adn_location)
        max_adn_location = g.get_max_location();
}

void Genotype::interprete(const std::vector<char>& adn)
{
    if(save_current_adn)
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

const Gene<int>& Genotype::get_gene_int(const std::string& s) const
{
    for(const Gene<int>& g : integer_genes)
        if(g.get_name() == s)
            return g;
    throw std::runtime_error("Error: no gene with id "+s);
}

const Gene<float>& Genotype::get_gene_float(const std::string& s) const
{
    for(const Gene<float>& g : float_genes)
        if(g.get_name() == s)
            return g;
    throw std::runtime_error("Error: no gene with id "+s);
}

size_t Genotype::get_number_integer_genes() const
{return integer_genes.size();}

size_t Genotype::get_number_float_genes() const
{return float_genes.size();}

int Genotype::get_min_location() const
{return min_adn_location;}

int Genotype::get_max_location() const
{return max_adn_location;}
