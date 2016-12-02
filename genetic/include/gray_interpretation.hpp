#ifndef GRAY_INTERPRETATION_HPP
#define GRAY_INTERPRETATION_HPP


#include <vector>
#include <map>


namespace Interpretation
{
    template <typename T = char, typename U = int>
    static U gray_interpretation(const std::vector<T>&, int, int);
    template <typename T, typename U = float, typename V = int>
    static U normed_gray_interpretation(const std::vector<T>&, int, int);
}


template <typename T, typename U>
U Interpretation::gray_interpretation(const std::vector<T>& adn, int min, int max)
{
    return 0;
}

template <typename T, typename U, typename V>
U Interpretation::normed_gray_interpretation(const std::vector<T>& adn, int min, int max)
{
    if(!impl::normalization_factors<U>.count(max-min))
        normed_binary_interpretation(adn, min, max);

    return (U)gray_interpretation<T, V>(adn, min, max)/impl::normalization_factors<U>[max-min];
}


#endif
