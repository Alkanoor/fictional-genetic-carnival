/* This test provides a proof of concept concerning interpretation functions on bits */

#include <iostream>

#include "utils/util.hpp"
#include "utils/vector_to_string_force_type.hpp"
#include "genetic/include/common_interpretations.hpp"


// g++ -std=c++14 -I../.. test.cpp $(echo $(ls ../../genetic/*.cpp)) $(echo $(ls ../../logger/*.cpp)) -o test -Wall -Werror

int main()
{
    auto f = Common_Interpretations::get_interpretation<int>("BINARY");
    auto g = Common_Interpretations::get_interpretation<float>("NORMED_BINARY");
    auto h = Common_Interpretations::get_interpretation<int>("GRAY");
    auto i = Common_Interpretations::get_interpretation<float>("NORMED_GRAY");
    auto j = Common_Interpretations::get_interpretation<float>("EXPONENTIAL");
    auto k = Common_Interpretations::get_interpretation<float>("NORMED_EXPONENTIAL");

    std::vector<char> bits = {0,1,0,0,1,1,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1};

    std::cout<<"Binary on string "<<Vector_To_String_Force_Type<int, std::vector<char> >(&bits).toString()<<" (len "<<bits.size()<<")"<<std::endl;
    std::cout<<f(bits, 0, 1)<<std::endl;
    std::cout<<f(bits, 0, 2)<<std::endl;
    std::cout<<f(bits, 1, 2)<<std::endl;
    std::cout<<f(bits, 0, 8)<<std::endl;
    std::cout<<f(bits, 8, 14)<<std::endl;
    std::cout<<f(bits, 14, 14+8)<<std::endl;
    std::cout<<f(bits, 22, 22+8)<<std::endl;

    std::cout<<"Normed binary on string "<<Vector_To_String_Force_Type<int, std::vector<char> >(&bits).toString()<<" (len "<<bits.size()<<")"<<std::endl;
    std::cout<<g(bits, 0, 1)<<std::endl;
    std::cout<<g(bits, 0, 2)<<std::endl;
    std::cout<<g(bits, 1, 2)<<std::endl;
    std::cout<<g(bits, 0, 8)<<std::endl;
    std::cout<<g(bits, 8, 14)<<std::endl;
    std::cout<<g(bits, 14, 14+8)<<std::endl;
    std::cout<<g(bits, 22, 22+8)<<std::endl;

    std::cout<<"Gray on string "<<Vector_To_String_Force_Type<int, std::vector<char> >(&bits).toString()<<" (len "<<bits.size()<<")"<<std::endl;
    std::cout<<h(bits, 0, 1)<<std::endl;
    std::cout<<h(bits, 0, 2)<<std::endl;
    std::cout<<h(bits, 1, 2)<<std::endl;
    std::cout<<h(bits, 0, 8)<<std::endl;
    std::cout<<h(bits, 8, 14)<<std::endl;
    std::cout<<h(bits, 14, 14+8)<<std::endl;
    std::cout<<h(bits, 22, 22+8)<<std::endl;

    std::cout<<"Normed gray on string "<<Vector_To_String_Force_Type<int, std::vector<char> >(&bits).toString()<<" (len "<<bits.size()<<")"<<std::endl;
    std::cout<<i(bits, 0, 1)<<std::endl;
    std::cout<<i(bits, 0, 2)<<std::endl;
    std::cout<<i(bits, 1, 2)<<std::endl;
    std::cout<<i(bits, 0, 8)<<std::endl;
    std::cout<<i(bits, 8, 14)<<std::endl;
    std::cout<<i(bits, 14, 14+8)<<std::endl;
    std::cout<<i(bits, 22, 22+8)<<std::endl;

    std::cout<<"Exponential on string "<<Vector_To_String_Force_Type<int, std::vector<char> >(&bits).toString()<<" (len "<<bits.size()<<")"<<std::endl;
    std::cout<<j(bits, 0, 1)<<std::endl;
    std::cout<<j(bits, 0, 2)<<std::endl;
    std::cout<<j(bits, 1, 2)<<std::endl;
    std::cout<<j(bits, 0, 8)<<std::endl;
    std::cout<<j(bits, 8, 14)<<std::endl;
    std::cout<<j(bits, 14, 14+8)<<std::endl;
    std::cout<<j(bits, 22, 22+8)<<std::endl;

    std::cout<<"Normed exponential on string "<<Vector_To_String_Force_Type<int, std::vector<char> >(&bits).toString()<<" (len "<<bits.size()<<")"<<std::endl;
    std::cout<<k(bits, 0, 1)<<std::endl;
    std::cout<<k(bits, 0, 2)<<std::endl;
    std::cout<<k(bits, 1, 2)<<std::endl;
    std::cout<<k(bits, 0, 8)<<std::endl;
    std::cout<<k(bits, 8, 14)<<std::endl;
    std::cout<<k(bits, 14, 14+8)<<std::endl;
    std::cout<<k(bits, 22, 22+8)<<std::endl;

    return 0;
}
