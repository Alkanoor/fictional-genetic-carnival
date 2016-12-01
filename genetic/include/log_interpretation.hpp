#ifndef LOG_INTERPRETATION_HPP
#define LOG_INTERPRETATION_HPP


#include <cassert>
#include <vector>
#include <map>


namespace Interpretation
{
    template <typename T, typename U = float>
    static float log_interpretation(const std::vector<T>&, int, int);
    template <typename T, typename U = float>
    static float normed_log_interpretation(const std::vector<T>&, int, int);
};


template <typename T, typename U>
U Interpretation::log_interpretation<T>(const std::vector<T>& adn, int min, int max)
{
    U res = 0;
    for(int i=min; i<max; i++)
        if(adn[i])
            res += log(i-min+1);
    return res;
}

namespace Interpretation
{
    namespace impl
    {
        template <typename U>
        std::map<int, U> normalization_factors;
    }

    template <typename T, typename U>
    U normed_log_interpretation(const std::vector<T>& adn, int min, int max)
    {
        if(!impl::normalization_factors<U>.count(max-min))
        {
            impl::normalization_factors<U>[max-min] = 0;
            for(int i=0; i<max-min; i++)
            {
                impl::normalization_factors<U>[max-min] += (U)log(i+1);
                assert(impl::normalization_factors<U>[max-min]>0);
            }
        }

        return log_interpretation<T, U>(adn, min, max)/impl::normalization_factors[max-min];
    }
}


#endif
