#include "include/common_interpretations.hpp"
#include "include/basic_interpretations.hpp"


std::map<std::string, std::function<int(const std::vector<char>&, int, int)> > Common_Interpretations::integer_representations;
std::map<std::string, std::function<float(const std::vector<char>&, int, int)> > Common_Interpretations::float_representations;

bool Common_Interpretations::initialized = false;


template <>
void Common_Interpretations::add_interpretation<int>(const std::string& name, const std::function<int(const std::vector<char>&, int, int)>& func)
{integer_representations[name] = func;}

template <>
void Common_Interpretations::add_interpretation<float>(const std::string& name, const std::function<float(const std::vector<char>&, int, int)>& func)
{float_representations[name] = func;}

void Common_Interpretations::init()
{
    if(!initialized)
    {
        initialized = true;

        integer_representations["BINARY"] = std::bind(&Interpretation::binary_interpretation<char, int>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        float_representations["NORMED_BINARY"] = std::bind(&Interpretation::normed_binary_interpretation<char, float, int>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        integer_representations["GRAY"] = std::bind(&Interpretation::gray_interpretation<char, int>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        float_representations["NORMED_GRAY"] = std::bind(&Interpretation::normed_gray_interpretation<char, float, int>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        float_representations["EXPONENTIAL"] = std::bind(&Interpretation::log_interpretation<char, float>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        float_representations["NORMED_EXPONENTIAL"] = std::bind(&Interpretation::normed_log_interpretation<char, float>, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    }
}

template <>
const std::function<int(const std::vector<char>&, int, int)>& Common_Interpretations::get_interpretation(const std::string& name)
{
    if(!initialized)
        init();
    return integer_representations[name];
}

template <>
const std::function<float(const std::vector<char>&, int, int)>& Common_Interpretations::get_interpretation(const std::string& name)
{
    if(!initialized)
        init();
    return float_representations[name];
}
