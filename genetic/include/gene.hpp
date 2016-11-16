#ifndef GENE_HPP
#define GENE_HPP


#include "common_interpretations.hpp"


template <size_t N, typename T>
class Gene
{
    public:
        Gene(const std::string& name, int min_adn_location, int max_adn_location, const std::string& interpretation_type="BINARY");

        void set_interpret_function(const std::function<T(const std::bitset<N>& adn, int, int)>&& interpret);
        T interprete(const std::bitset<N>& adn);
        T get_current_interpretation() const;

        const std::string& get_name() const;

    private:
        std::string name;
        int min_adn_location;
        int max_adn_location;

        T current_interpretation;

        std::function<T(const std::bitset<N>& adn, int, int)> &interpret_function;
};


#endif
