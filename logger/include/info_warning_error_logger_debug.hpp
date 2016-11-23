/**
Copyright (C)

creation date : 08/11/2016
file : info_warning_error_logger_debug.hpp
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


#ifndef INFO_WARNING_ERROR_LOGGER_DEBUG_HPP
#define INFO_WARNING_ERROR_LOGGER_DEBUG_HPP


#include "info_warning_error_logger.hpp"


class Info_Warning_Error_Logger_Debug : public Info_Warning_Error_Logger
{
    public:
        Info_Warning_Error_Logger_Debug(bool debug = true, bool writeDebug = true, bool date = true, int type = -1);

        void toggleDebug();
        void setDebug(bool debug);
        void setWriteDebug(bool writeDebug);
        bool isDebug() const;
        bool isWriteDebug() const;

        template <typename T>
        Info_Warning_Error_Logger_Debug& operator << (T args);

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

        static std::shared_ptr<Info_Warning_Error_Logger_Debug> static_log;

    private:
        bool debug;
        bool writeDebug;
};


template <typename T>
Info_Warning_Error_Logger_Debug& Info_Warning_Error_Logger_Debug::operator << (T args)
{
    if(debug)
        *((Info_Warning_Error_Logger*)this)<<args;
    return *this;
}

template <typename ... T>
void Info_Warning_Error_Logger_Debug::write(T... args)
{
    if(debug)
    {
        if(writeDebug)
            for(std::vector<std::shared_ptr<Handler> >::iterator it = handlers.begin(); it != handlers.end(); it++)
                (*it)->write("|DEBUG| ");
        ((Info_Warning_Error_Logger*)this)->write(args ...);
    }
}

template <typename ... T>
void Info_Warning_Error_Logger_Debug::info(T... args)
{
    if(debug)
    {
        if(writeDebug)
            for(std::vector<std::shared_ptr<Handler> >::iterator it = handlers.begin(); it != handlers.end(); it++)
                (*it)->write("|DEBUG| ");
        ((Info_Warning_Error_Logger*)this)->info(args ...);
    }
}

template <typename ... T>
void Info_Warning_Error_Logger_Debug::warning(T... args)
{
    if(debug)
    {
        if(writeDebug)
            for(std::vector<std::shared_ptr<Handler> >::iterator it = handlers.begin(); it != handlers.end(); it++)
                (*it)->write("|DEBUG| ");
        ((Info_Warning_Error_Logger*)this)->warning(args ...);
    }
}

template <typename ... T>
void Info_Warning_Error_Logger_Debug::error(T... args)
{
    if(debug)
    {
        if(writeDebug)
            for(std::vector<std::shared_ptr<Handler> >::iterator it = handlers.begin(); it != handlers.end(); it++)
                (*it)->write("|DEBUG| ");
        ((Info_Warning_Error_Logger*)this)->error(args ...);
    }
}


#endif
