#include "include/genotype.hpp"


Genotype::Genotype() :
    max_location(0)
{}

void Genotype::interprete(const std::vector<char>& adn)
{
    current_gene = adn;
    if((int)adn.size() < max_location)
        throw std::runtime_error("Error: impossible interpretation: not enough bits on adn");

    for(int i=0; i<(int)integer_interpretations.size(); i++)
        integer_interpreted[i] = integer_interpretations[i](adn, min_location_integer_genes[i], max_location_integer_genes[i]);
    for(int i=0; i<(int)float_interpretations.size(); i++)
        float_interpreted[i] = float_interpretations[i](adn, min_location_float_genes[i], max_location_float_genes[i]);
}

const std::vector<int>& Genotype::get_integer_interpreted() const
{return integer_interpreted;}

const std::vector<float>& Genotype::get_float_interpreted() const
{return float_interpreted;}
