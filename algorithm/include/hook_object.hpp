#ifndef HOOK_OBJECT_HPP
#define HOOK_OBJECT_HPP


#include <vector>
#include <array>


template <typename T, size_t Population_size>
class Hook_Object
{
    public:
        void new_step();
        void set(const std::array<std::vector<char>, Population_size>& bits);
        void set(const std::array<T, Population_size>& evaluation);
};

template <typename T, size_t Population_size>
void Hook_Object<T, Population_size>::new_step()
{}

template <typename T, size_t Population_size>
void Hook_Object<T, Population_size>::set(const std::array<std::vector<char>, Population_size>& bits)
{(void)bits;}

template <typename T, size_t Population_size>
void Hook_Object<T, Population_size>::set(const std::array<T, Population_size>& evaluation)
{(void)evaluation;}


#endif
