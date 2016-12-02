#ifndef LOG_INTERPRETATION_HPP
#define LOG_INTERPRETATION_HPP


#include <cassert>
#include <vector>
#include <cmath>
#include <map>


namespace Interpretation
{
    template <typename T, typename U = float>
    static U log_interpretation(const std::vector<T>&, int, int);
    template <typename T, typename U = float>
    static U normed_log_interpretation(const std::vector<T>&, int, int);
};


template <typename T, typename U>
U Interpretation::log_interpretation(const std::vector<T>& adn, int min, int max)
{
    U res = 0;
    for(int i=min; i<max; i++)
        if(adn[i])
            res += log(i-min+1.1);
    return res;
}

namespace Interpretation
{
    namespace impl
    {
        template <typename U>
        std::map<int, U> normalization_factors_log;
    }

    template <typename T, typename U>
    U normed_log_interpretation(const std::vector<T>& adn, int min, int max)
    {
        if(!impl::normalization_factors_log<U>.count(max-min))
        {
            impl::normalization_factors_log<U>[max-min] = 0;
            for(int i=0; i<max-min; i++)
            {
                impl::normalization_factors_log<U>[max-min] += (U)log(i+1.1);
                assert(impl::normalization_factors_log<U>[max-min]>0);
            }
        }

        return log_interpretation<T, U>(adn, min, max)/impl::normalization_factors_log<U>[max-min];
    }
}


#endif
