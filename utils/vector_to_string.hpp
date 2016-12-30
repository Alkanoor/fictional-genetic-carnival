#ifndef VECTOR_TO_STRING_HPP
#define VECTOR_TO_STRING_HPP


#include <string>
#include <sstream>


template <typename T>
class Vector_To_String
{
    public:
        Vector_To_String(T v, const std::string& separator=", ", const std::string& begin="[", const std::string& end="]");
        Vector_To_String(const T* v, const std::string& separator=", ", const std::string& begin="[", const std::string& end="]");

        const std::string& toString() const;

    private:
        std::string represented;
};


template <typename T>
Vector_To_String<T>::Vector_To_String(T v, const std::string& separator, const std::string& begin, const std::string& end)
{
    int l = (int)(v.size());
    represented += begin;
    for(int i=0; i<l; i++)
    {
        std::ostringstream os;
        os<<v[i];
        represented += os.str();
        if(i+1<l)
            represented += separator;
    }
    represented += end;
}

template <typename T>
Vector_To_String<T>::Vector_To_String(const T* v, const std::string& separator, const std::string& begin, const std::string& end)
{
    int l = (int)v->size();
    represented += begin;
    for(int i=0; i<l; i++)
    {
        std::ostringstream os;
        os<<(*v)[i];
        represented += os.str();
        if(i+1<l)
            represented += separator;
    }
    represented += end;
}

template <typename T>
const std::string& Vector_To_String<T>::toString() const
{return represented;}


#endif
