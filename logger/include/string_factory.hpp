/**
Copyright (C)

creation date : 19/03/2015
file : string_factory.hpp
author : Julien Schoumacher

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**/


#ifndef STRING_FACTORY_HPP
#define STRING_FACTORY_HPP


#include <string>
#include <sstream>


class String_Factory
{
    public:
        template <typename T>
        static std::string toString(T val);
        template <typename T, typename ... U>
        static std::string toString(T val, U ... next);
};


template <typename T, typename ... U>
std::string String_Factory::toString(T val, U ... next)
{
    std::string temp = String_Factory::toString(val);
    return temp+String_Factory::toString(next...);
}

template <typename T>
std::string String_Factory::toString(T val)
{
    return val.toString();
}

template <>
inline std::string String_Factory::toString(const std::string &val)
{
    return val;
}

template <>
inline std::string String_Factory::toString(std::string val)
{
    return val;
}

template <>
inline std::string String_Factory::toString(bool val)
{
    if(val)
        return "true";
    else
        return "false";
}

template <>
inline std::string String_Factory::toString(unsigned char val)
{
    std::ostringstream ofs;
    ofs<<(int)val;
    return ofs.str();
}

template <>
inline std::string String_Factory::toString(char val)
{
    std::ostringstream ofs;
    ofs<<val;
    return ofs.str();
}

template <>
inline std::string String_Factory::toString(int val)
{
    std::ostringstream ofs;
    ofs<<val;
    return ofs.str();
}

template <>
inline std::string String_Factory::toString(unsigned int val)
{
    std::ostringstream ofs;
    ofs<<val;
    return ofs.str();
}

template <>
inline std::string String_Factory::toString(float val)
{
    std::ostringstream ofs;
    ofs<<val;
    return ofs.str();
}

template <>
inline std::string String_Factory::toString(char* val)
{
    return std::string(val);
}

template <>
inline std::string String_Factory::toString(const char* val)
{
    return std::string(val);
}

template <>
inline std::string String_Factory::toString(const unsigned char* val)
{
    char* conv = (char*)val;
    return std::string(conv);
}


#endif
