#ifndef GENE_HPP
#define GENE_HPP


#include <cassert>

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

        T interprete(const std::vector<char>& adn) throw ();
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
Gene<T>::Gene(const std::string& name, int min_adn_location, int max_adn_location, const std::string& interpretation_type) :
    name(name),
    min_adn_location(min_adn_location),
    max_adn_location(max_adn_location),
    interpret_function(Common_Interpretations::get_interpretation<T>(interpretation_type))
{
    assert(min_adn_location >= 0 && max_adn_location > min_adn_location);
    if(!interpret_function)
        throw std::runtime_error("Error: Unexisting interpretation function of type "+interpretation_type);
}

template <typename T>
Gene<T>::Gene(const std::string& name, int min_adn_location, int max_adn_location, const std::function<T(const std::vector<char>& adn, int, int)>& interpretation) :
    name(name),
    min_adn_location(min_adn_location),
    max_adn_location(max_adn_location),
    interpret_function(interpretation)
{
    assert(min_adn_location >= 0 && max_adn_location > min_adn_location);
    if(!interpret_function)
        throw std::runtime_error("Error: Unexisting interpretation function passed as argument");
}

template <typename T>
T Gene<T>::interprete(const std::vector<char>& adn) throw ()
{
    if((int)adn.size() < max_adn_location)
        throw std::runtime_error("Error: impossible interpretation: not enough bits on adn");

    current_interpretation = interpret_function(adn, min_adn_location, max_adn_location);

    return current_interpretation;
}

template <typename T>
T Gene<T>::get_current_interpretation() const
{return current_interpretation;}

template <typename T>
const std::string& Gene<T>::get_name() const
{return name;}


#endif
