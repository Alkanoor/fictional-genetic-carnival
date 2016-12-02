#ifndef VECTOR_TO_STRING_FORCE_TYPE_HPP
#define VECTOR_TO_STRING_FORCE_TYPE_HPP


#include <string>
#include <sstream>


template <typename S, typename T>
class Vector_To_String_Force_Type
{
    public:
        Vector_To_String_Force_Type(T v, const std::string& separator=", ", const std::string& begin="[", const std::string& end="]");
        Vector_To_String_Force_Type(const T* v, const std::string& separator=", ", const std::string& begin="[", const std::string& end="]");

        const std::string& toString() const;

    private:
        std::string represented;
};


template <typename S, typename T>
Vector_To_String_Force_Type<S,T>::Vector_To_String_Force_Type(T v, const std::string& separator, const std::string& begin, const std::string& end)
{
    int l = (int)v.size();
    represented += begin;
    for(int i=0; i<l; i++)
    {
        std::ostringstream os;
        os<<(S)v[i];
        represented += os.str();
        if(i+1<l)
            represented += separator;
    }
    represented += end;
}

template <typename S, typename T>
Vector_To_String_Force_Type<S,T>::Vector_To_String_Force_Type(const T* v, const std::string& separator, const std::string& begin, const std::string& end)
{
    int l = (int)v->size();
    represented += begin;
    for(int i=0; i<l; i++)
    {
        std::ostringstream os;
        os<<(S)(*v)[i];
        represented += os.str();
        if(i+1<l)
            represented += separator;
    }
    represented += end;
}

template <typename S, typename T>
const std::string& Vector_To_String_Force_Type<S,T>::toString() const
{return represented;}


#endif
