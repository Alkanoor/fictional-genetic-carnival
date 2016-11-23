/**
Copyright (C)

creation date : 23/11/2016
file : info_warning_error_logger_threaded.hpp
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


#ifndef INFO_WARNING_ERROR_LOGGER_THREADED_HPP
#define INFO_WARNING_ERROR_LOGGER_THREADED_HPP


#include "info_warning_error_logger.hpp"

#include <mutex>


class Info_Warning_Error_Logger_Threaded : public Info_Warning_Error_Logger
{
    public:
        Info_Warning_Error_Logger_Threaded(bool date = true, int type = -1);

        template <typename T>
        Info_Warning_Error_Logger_Threaded& operator << (T args);

        template <typename ... T>
        void write(T... args);
        template <typename ... T>
        void info(T... args);
        template <typename ... T>
        void warning(T... args);
        template <typename ... T>
        void error(T... args);
        void printDate();
        void endLine();

        static std::string type();

        static std::shared_ptr<Info_Warning_Error_Logger_Threaded> static_log;

    private:
        std::mutex mutex;
};


template <typename T>
Info_Warning_Error_Logger_Threaded& Info_Warning_Error_Logger_Threaded::operator << (T args)
{
    mutex.lock();
    *((Info_Warning_Error_Logger*)this)<<args;
    mutex.unlock();
    return *this;
}

template <typename ... T>
void Info_Warning_Error_Logger_Threaded::write(T... args)
{
    mutex.lock();
    ((Info_Warning_Error_Logger*)this)->write(args ...);
    mutex.unlock();
}

template <typename ... T>
void Info_Warning_Error_Logger_Threaded::info(T... args)
{
    mutex.lock();
    ((Info_Warning_Error_Logger*)this)->info(args ...);
    mutex.unlock();
}

template <typename ... T>
void Info_Warning_Error_Logger_Threaded::warning(T... args)
{
    mutex.lock();
    ((Info_Warning_Error_Logger*)this)->warning(args ...);
    mutex.unlock();
}

template <typename ... T>
void Info_Warning_Error_Logger_Threaded::error(T... args)
{
    mutex.lock();
    ((Info_Warning_Error_Logger*)this)->error(args ...);
    mutex.unlock();
}


#endif
