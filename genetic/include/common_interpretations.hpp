#ifndef COMMON_INTERPRETATION_HPP
#define COMMON_INTERPRETATION_HPP


#include <bitset>


template <size_t N>
class Common_Interpretations
{
    public:
        template <typename T> // T is either an int or a float
        static const std::function<T(const std::bitset<N>&, int, int)>& get_interpretation(const std::string& name);

        static int binary_interpretation(const std::bitset<N>&, int, int);
        static int gray_interpretation(const std::bitset<N>&, int, int);

        static float log_interpretation(const std::bitset<N>&, int, int);

        static void init();

    private:
        static std::map<std::string, std::function<int(const std::bitset<N>&, int, int)> > integer_representations;
        static std::map<std::string, std::function<float(const std::bitset<N>&, int, int)> > float_representations;

        static bool initialized;
};


template <size_t N>
Common_Interpretations<N>::


const std::function<T(const std::bitset<N>&, int, int)>& Common_Interpretations<N>::get_interpretation(const std::string& name);

int Common_Interpretations<N>::binary_interpretation(const std::bitset<N>& adn, int min, int max)
{}

int Common_Interpretations<N>::gray_interpretation(int Common_Interpretations<N>::binary_interpretation(const std::bitset<N>& adn, int min, int max)
{}

float Common_Interpretations<N>::log_interpretation(const std::bitset<N>& adn, int min, int max)
{
    float res = 0;
    for(int i=min; i<max; i++)
        if(adn[i])
            res += log(i-min+1);
    return res;
}

void Common_Interpretations<N>::init()
{
    integer_interpretations["BINARY"] = std::bind(&Common_Interpretations<N>::binary_interpretation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    integer_interpretations["GRAY"] = std::bind(&Common_Interpretations<N>::gray_interpretation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    float_interpretations["EXPONENTIAL"] = std::bind(&Common_Interpretations<N>::log_interpretation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}


#endif
