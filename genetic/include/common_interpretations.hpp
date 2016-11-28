#ifndef COMMON_INTERPRETATION_HPP
#define COMMON_INTERPRETATION_HPP


#include <bitset>


class Common_Interpretations
{
    public:
        template <typename T> // T is either an int or a float
        static void add_interpretation(const std::string& name, const std::function<T(const std::vector<char>&, int, int)>& func);

        template <typename T> // T is either an int or a float
        static const std::function<T(const std::vector<char>&, int, int)>& get_interpretation(const std::string& name);

        static void init();

    private:
        static std::map<std::string, std::function<int(const std::vector<char>&, int, int)> > integer_representations;
        static std::map<std::string, std::function<float(const std::vector<char>&, int, int)> > float_representations;

        static bool initialized;
};


template <>
const std::function<int(const std::vector<char>&, int, int)>& Common_Interpretations<N>::get_interpretation(const std::string& name)
{return integer_interpretations[name];}

template <>
const std::function<float(const std::vector<char>&, int, int)>& Common_Interpretations<N>::get_interpretation(const std::string& name)
{return float_interpretations[name];}

template <size_t N>
void Common_Interpretations<N>::init()
{
    initialized = true;

    integer_interpretations["BINARY"] = std::bind(&Common_Interpretations<N>::binary_interpretation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    integer_interpretations["GRAY"] = std::bind(&Common_Interpretations<N>::gray_interpretation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    float_interpretations["EXPONENTIAL"] = std::bind(&Common_Interpretations<N>::log_interpretation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}


#endif
