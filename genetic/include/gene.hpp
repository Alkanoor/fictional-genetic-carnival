#ifndef GENE_HPP
#define GENE_HPP


#include "common_interpretations.hpp"


///*****************************************************************************************
/// Class that provides gene model : association of name, bit range and interpretation
///*****************************************************************************************

template <typename T>
class Gene
{
    public:
        Gene(const std::string& name, int min_adn_location, int max_adn_location, const std::string& interpretation_type="BINARY");
        Gene(const std::string& name, int min_adn_location, int max_adn_location, const std::function<T(const std::vector<char>& adn, int, int)>& interpretation);

        T interprete(const std::vector<char>& adn);
        T get_current_interpretation() const;

        const std::string& get_name() const;

    private:
        std::string name;
        int min_adn_location;
        int max_adn_location;

        T current_interpretation;

        const std::function<T(const std::vector<char>& adn, int, int)>& interpret_function;
};


template <typename T>
Gene<T>::Gene(const std::string& name, int min_adn_location, int max_adn_location, const std::string& interpretation_type="BINARY") :
    name(name),
    min_adn_location(min_adn_location),
    max_adn_location(max_adn_location),
    interpret_function(Common_Interpretations::get_interpretation(interpretation_type))
{
    assert(min_adn_location >= 0 && max_adn_location > min_adn_location);
}

template <typename T>
Gene<T>::Gene(const std::string& name, int min_adn_location, int max_adn_location, const std::function<T(const std::vector<char>& adn, int, int)>& interpretation) :
    name(name),
    min_adn_location(min_adn_location),
    max_adn_location(max_adn_location),
    interpret_function(interpretation)
{
    assert(min_adn_location >= 0 && max_adn_location > min_adn_location);
}


template <typename T>
T Gene<T>::interprete(const std::vector<char>& adn)
{
    if((int)adn.size() < max_adn_location)
        throw std::runtime_error("Error: impossible interpretation: not enough bits on adn");

    current_interpretation = interpret_function(adn, min_adn_location, max_adn_location);
}

template <typename T>
T Gene<T>::get_current_interpretation() const
{return current_interpretation;}

template <typename T>
const std::string& Gene<T>::get_name() const
{return name;}


#endif
