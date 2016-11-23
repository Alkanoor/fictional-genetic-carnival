/**
Copyright (C)

creation date : 05/11/2015
file : info_warning_error_logger.hpp
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


#ifndef INFO_WARNING_ERROR_LOGGER_HPP
#define INFO_WARNING_ERROR_LOGGER_HPP


#include <ctime>

#include "string_factory.hpp"
#include "logger.hpp"


class Info_Warning_Error_Logger : public Logger
{
    public:
        Info_Warning_Error_Logger(bool date = true, int type = -1);

    	void setDateAuto(bool date = true);
    	void setEndlineAuto(bool endline = true);
    	void setType(int type = -1);
        void setType(const std::string& type);

        template <typename T>
        Info_Warning_Error_Logger& operator << (T args);

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

        static std::shared_ptr<Info_Warning_Error_Logger> static_log;

    private:
        bool endLineAuto;
        bool printDateAuto;
        int currentType;
};


template <typename T>
Info_Warning_Error_Logger& Info_Warning_Error_Logger::operator << (T args)
{
    bool tempEndLineAuto = endLineAuto;
    bool temp = printDateAuto;
    endLineAuto = false;
    printDateAuto = false;
    if(currentType==0)
        info(args);
    else if(currentType==1)
        warning(args);
    else if(currentType==2)
        error(args);
    else
        write(args);
    endLineAuto = tempEndLineAuto;
    printDateAuto = temp;
    return *this;
}

template <typename ... T>
void Info_Warning_Error_Logger::write(T... args)
{
    for(std::vector<std::shared_ptr<Handler> >::iterator it = handlers.begin(); it != handlers.end(); it++)
    {
        if(printDateAuto)
            (**it)<<String_Factory::toString('<')<<date()<<"> ";
        (**it)<<String_Factory::toString(args...);
        if(endLineAuto)
            (*it)->writeEndline("");
    }
}

template <typename ... T>
void Info_Warning_Error_Logger::info(T... args)
{
    for(std::vector<std::shared_ptr<Handler> >::iterator it = handlers.begin(); it != handlers.end(); it++)
    {
        if(printDateAuto)
            (**it)<<String_Factory::toString('<')<<date()<<"> ";
        (**it)<<"[Info] : "<<String_Factory::toString(args...);
        if(endLineAuto)
            (*it)->writeEndline("");
    }
}

template <typename ... T>
void Info_Warning_Error_Logger::warning(T... args)
{
    for(std::vector<std::shared_ptr<Handler> >::iterator it = handlers.begin(); it != handlers.end(); it++)
    {
        if(printDateAuto)
            (**it)<<String_Factory::toString('<')<<date()<<"> ";
        (**it)<<"[Warning] : "<<String_Factory::toString(args...);
        if(endLineAuto)
            (*it)->writeEndline("");
    }
}

template <typename ... T>
void Info_Warning_Error_Logger::error(T... args)
{
    for(std::vector<std::shared_ptr<Handler> >::iterator it = handlers.begin(); it != handlers.end(); it++)
    {
        if(printDateAuto)
            (**it)<<String_Factory::toString('<')<<date()<<"> ";
        (**it)<<"[Error] : "<<String_Factory::toString(args...);
        if(endLineAuto)
            (*it)->writeEndline("");
    }
}


#endif
