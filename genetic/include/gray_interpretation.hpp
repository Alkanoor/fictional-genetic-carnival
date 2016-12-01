#ifndef GRAY_INTERPRETATION_HPP
#define GRAY_INTERPRETATION_HPP


#include <vector>
#include <map>


namespace Interpretation
{
    template <typename T = char, typename U = int>
    static int gray_interpretation(const std::vector<T>&, int, int);
    template <typename T, typename U = float, typename V = int>
    static float normed_gray_interpretation(const std::vector<T>&, int, int);
}


template <typename T, typename U>
U Interpretation::gray_interpretation(const std::vector<T>& adn, int min, int max)
{
    return 0;
}

namespace Interpretation
{
    namespace impl
    {
        template <typename U>
        std::map<int, U> normalization_factors;
    }

    template <typename T, typename U, typename V>
    U normed_gray_interpretation(const std::vector<T>& adn, int min, int max)
    {
        if(!impl::normalization_factors<U>.count(max-min))
        {
            impl::normalization_factors<U>[max-min] = 1;
            for(int i=0; i<max-min; i++)
            {
                impl::normalization_factors<U>[max-min] *= 2;
                assert(impl::normalization_factors<U>[max-min]-1>0);
            }
            impl::normalization_factors<U>[max-min]--;
        }
        else
            return (U)gray_interpretation<T, V>(adn, min, max)/impl::normalization_factors<U>[max-min];
    }
}


#endif
