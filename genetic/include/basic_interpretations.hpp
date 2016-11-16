#ifndef BASIC_INTERPRETATION_HPP
#define BASIC_INTERPRETATION_HPP


#include <map>
#include <bitset>


namespace Binary_Interpretation
{
    template <typename T>
    static int binary_interpretation(const std::vector<T>&, int, int);
    template <typename T>
    static float normed_binary_interpretation(const std::vector<T>&, int, int);
}

namespace Gray_Interpretation
{
    template <typename T>
    static int gray_interpretation(const std::vector<T>&, int, int);
    template <typename T>
    static float normed_gray_interpretation(const std::vector<T>&, int, int);
}

namespace Log_Interpretation
{
    template <typename T>
    static float log_interpretation(const std::vector<T>&, int, int);
    template <typename T>
    static float normed_log_interpretation(const std::vector<T>&, int, int);
};


namespace Binary_Interpretation
{
    template <typename T = char, typename U = int>
    U Binary_Interpretation::binary_interpretation<T>(const std::vector<T>& adn, int min, int max)
    {
        U r = 0;
        U a = 1;
        for(int i=min; i<max; i++, a<<=1)
            if(adn[i])
                r += a;
        return r;
    }

    namespace
    {
        template <typename U>
        std::map<int, U> normalization_factors;
    }

    template <typename T = char, typename U = float, typename V = int>
    U Binary_Interpretation::normed_binary_interpretation<T>(const std::vector<T>& adn, int min, int max)
    {
        if(!normalization_factors<U>.count(max-min))
        {
            normalization_factors<U>[max-min] = 1;
            for(int i=0; i<max-min; i++)
            {
                normalization_factors<U>[max-min] *= 2;
                assert(normalization_factors<U>[max-min]-1>0);
            }
            normalization_factors<U>[max-min]--;
        }
        else
            return (U)binary_interpretation<T, V>(adn, min, max)/normalization_factors<U>[max-min];
    }
}


namespace Gray_Interpretation
{
    template <typename T = char, typename U = int>
    U Gray_Interpretation::gray_interpretation<T>(const std::vector<T>& adn, int min, int max)
    {
        return 0;
    }

    template <typename T, typename U = float, typename V = int>
    U Gray_Interpretation::normed_gray_interpretation<T>(const std::vector<T>& adn, int min, int max)
    {
        return 0;
    }
}


namespace Log_Interpretation
{
    template <typename T, typename U = float>
    U Log_Interpretation::log_interpretation<T>(const std::vector<T>& adn, int min, int max)
    {
        U res = 0;
        for(int i=min; i<max; i++)
            if(adn[i])
                res += log(i-min+1);
        return res;
    }

    namespace
    {
        template <typename U>
        std::map<int, U> normalization_factors;
    }

    template <typename T, typename U = float>
    static U Log_Interpretation<T>::normed_log_interpretation(const std::vector<T>& adn, int min, int max)
    {
        if(!normalization_factors<U>.count(max-min))
        {
            normalization_factors<U>[max-min] = 0;
            for(int i=0; i<max-min; i++)
            {
                normalization_factors<U>[max-min] += (U)log(i+1);
                assert(normalization_factors<U>[max-min]>0);
            }
        }

        return log_interpretation<T, U>(adn, min, max)/normalization_factors[max-min];
    }
}


#endif
