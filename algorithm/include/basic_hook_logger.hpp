#ifndef BASIC_HOOK_LOGGER_OBJECT_HPP
#define BASIC_HOOK_LOGGER_OBJECT_HPP


#include <stdexcept>

#include "hook_object.hpp"


template <typename T, size_t Population_size>
class Basic_Hook_Logger : public Hook_Object<T, Population_size>
{
    public:
        Basic_Hook_Logger(const std::shared_ptr<Info_Warning_Error_Logger_Threaded>& log = Easy_Log_In_File_Threaded::getInfoLog());

        void new_step();
        void set(const std::array<std::vector<char>, Population_size>& bits);
        void set(const std::array<T, Population_size>& evaluation);

    private:
        std::shared_ptr<Info_Warning_Error_Logger> logger;
};


template <typename T, size_t Population_size>
Basic_Hook_Logger<T, Population_size>::Basic_Hook_Logger(const std::shared_ptr<Info_Warning_Error_Logger_Threaded>& log) :
    logger(log)
{}

template <typename T, size_t Population_size>
void Basic_Hook_Logger<T, Population_size>::new_step()
{}

template <typename T, size_t Population_size>
void Basic_Hook_Logger<T, Population_size>::set(const std::array<std::vector<char>, Population_size>& bits)
{
    if(!logger)
        throw std::runtime_error("Error: no logger specified in basic hook");

    logger->write("Current population :");
    for(const std::vector<char>& i : bits)
    {
        (*logger)<<Vector_To_String_Force_Type<int, const std::vector<char> >(&i);
        logger->endLine();
    }
}

template <typename T, size_t Population_size>
void Basic_Hook_Logger<T, Population_size>::set(const std::array<T, Population_size>& evaluation)
{
    if(!logger)
        throw std::runtime_error("Error: no logger specified in basic hook");

    logger->write("Current evaluation : ", Vector_To_String<const std::array<T, Population_size> >(&evaluation));
}


#endif
