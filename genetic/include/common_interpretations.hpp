#ifndef COMMON_INTERPRETATIONS_HPP
#define COMMON_INTERPRETATIONS_HPP


#include <functional>
#include <vector>
#include <map>


///**********************************************************************************
/// Class (to keep encapsulation) that provides a pool of interpretation functions
///**********************************************************************************

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


#endif
