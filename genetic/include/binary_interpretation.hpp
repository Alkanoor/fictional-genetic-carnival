#ifndef BINARY_INTERPRETATION_HPP
#define BINARY_INTERPRETATION_HPP


#include <cassert>
#include <vector>
#include <map>


namespace Interpretation
{
    template <typename T = char, typename U = int>
    static U binary_interpretation(const std::vector<T>&, int, int);
    template <typename T = char, typename U = float, typename V = int>
    static U normed_binary_interpretation(const std::vector<T>&, int, int);
}


template <typename T, typename U>
U Interpretation::binary_interpretation(const std::vector<T>& adn, int min, int max)
{
    U r = 0;
    U a = 1;
    for(int i=min; i<max; i++, a<<=1)
        if(adn[i])
            r += a;
    return r;
}

namespace Interpretation
{
    namespace impl
    {
        template <typename U>
        std::map<int, U> normalization_factors;
    }

    template <typename T, typename U, typename V>
    U normed_binary_interpretation(const std::vector<T>& adn, int min, int max)
    {
        if(!impl::normalization_factors<U>.count(max-min))
        {
            impl::normalization_factors<U>[max-min] = 1;
            for(int i=0; i<max-min; i++)
            {
                impl::normalization_factors<U>[max-min] *= 2;
                assert(impl::normalization_factors<U>[max-min]>1);
            }
            if(max-min > 0)
                impl::normalization_factors<U>[max-min]--;
        }
        
        return (U)binary_interpretation<T, V>(adn, min, max)/impl::normalization_factors<U>[max-min];
    }
}


#endif
