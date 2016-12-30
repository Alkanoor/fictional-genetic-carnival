#ifndef HOOK_OBJECT_HPP
#define HOOK_OBJECT_HPP


#include <vector>
#include <array>


template <typename T, size_t Population_size>
class Hook_Object
{
    public:
        virtual void new_step() = 0;
        virtual void set(const std::array<std::vector<char>, Population_size>& bits) = 0;
        virtual void set(const std::array<T, Population_size>& evaluation) = 0;
};


#endif
