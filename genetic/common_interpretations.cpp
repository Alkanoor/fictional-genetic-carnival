#include "common_interpretations.hpp"


std::map<std::string, std::function<int(const std::vector<char>&, int, int)> > Common_Interpretations::integer_representations;
std::map<std::string, std::function<float(const std::vector<char>&, int, int)> > Common_Interpretations::float_representations;

bool Common_Interpretations::initialized = false;


void Common_Interpretations::init()
{
    if(!initialized)
    {
        initialized = true;

        integer_interpretations["BINARY"] = std::bind(&Interpretation::binary_interpretation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        integer_interpretations["NORMED_BINARY"] = std::bind(&Interpretation::normed_binary_interpretation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        integer_interpretations["GRAY"] = std::bind(&Interpretation::gray_interpretation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        integer_interpretations["NORMED_GRAY"] = std::bind(&Interpretation::normed_gray_interpretation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        float_interpretations["EXPONENTIAL"] = std::bind(&Interpretation::log_interpretation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        float_interpretations["NORMED_EXPONENTIAL"] = std::bind(&Interpretation::normed_log_interpretation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    }
}
