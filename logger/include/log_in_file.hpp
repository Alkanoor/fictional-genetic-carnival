/**
Copyright (C)

creation date : 08/11/2015
file : log_in_file.hpp
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


#ifndef LOG_IN_FILE_HPP
#define LOG_IN_FILE_HPP


#include "info_warning_error_logger_threaded_debug.hpp"
#include "info_warning_error_logger_threaded.hpp"
#include "info_warning_error_logger_debug.hpp"
#include "info_warning_error_logger.hpp"
#include "file_handler.hpp"


template <typename Logger_Type>
class Log_In_File
{
    public:
        static Log_In_File<Logger_Type>& getInstance();

        static std::shared_ptr<Logger_Type> getInfoLog();
        static std::shared_ptr<Logger_Type> getWarningLog();
        static std::shared_ptr<Logger_Type> getErrorLog();

        static void setFolderPath(const std::string& path);
        static void close();

    private:
        Log_In_File(const std::string& folderPath);
        Log_In_File(const Log_In_File& cpy);
        Log_In_File& operator = (const Log_In_File& noCpy);

        static std::shared_ptr<Log_In_File<Logger_Type> > instance;

        std::shared_ptr<Logger_Type> infoLog;
        std::shared_ptr<Logger_Type> warningLog;
        std::shared_ptr<Logger_Type> errorLog;

        std::string path;
        std::string infoPath;
        std::string warningPath;
        std::string errorPath;
};

typedef Log_In_File<Info_Warning_Error_Logger> Easy_Log_In_File;
typedef Log_In_File<Info_Warning_Error_Logger_Debug> Easy_Log_In_File_Debug;
typedef Log_In_File<Info_Warning_Error_Logger_Threaded> Easy_Log_In_File_Threaded;
typedef Log_In_File<Info_Warning_Error_Logger_Threaded_Debug> Easy_Log_In_File_Threaded_Debug;


template <typename Logger_Type>
std::shared_ptr<Log_In_File<Logger_Type> > Log_In_File<Logger_Type>::instance(new Log_In_File<Logger_Type>("logs/"));

template <typename Logger_Type>
Log_In_File<Logger_Type>& Log_In_File<Logger_Type>::getInstance()
{return *instance;}

template <typename Logger_Type>
std::shared_ptr<Logger_Type> Log_In_File<Logger_Type>::getInfoLog()
{return instance->infoLog;}

template <typename Logger_Type>
std::shared_ptr<Logger_Type> Log_In_File<Logger_Type>::getWarningLog()
{return instance->warningLog;}

template <typename Logger_Type>
std::shared_ptr<Logger_Type> Log_In_File<Logger_Type>::getErrorLog()
{return instance->errorLog;}

template <typename Logger_Type>
void Log_In_File<Logger_Type>::setFolderPath(const std::string& path)
{
    if(path!=instance->path)
        instance.reset(new Log_In_File<Logger_Type>(path));
}

template <typename Logger_Type>
void Log_In_File<Logger_Type>::close()
{
    instance->infoLog.reset();
    instance->warningLog.reset();
    instance->errorLog.reset();
}

template <typename Logger_Type>
Log_In_File<Logger_Type>::Log_In_File(const std::string& folderPath) :
    infoLog(std::shared_ptr<Logger_Type>(new Logger_Type())),
    warningLog(std::shared_ptr<Logger_Type>(new Logger_Type())),
    errorLog(std::shared_ptr<Logger_Type>(new Logger_Type()))
{
    path = folderPath;
    infoPath = folderPath+Logger::date()+"."+Logger_Type::type()+".infoLog";
    warningPath = folderPath+Logger::date()+"."+Logger_Type::type()+".warningLog";
    errorPath = folderPath+Logger::date()+"."+Logger_Type::type()+".errorLog";
    infoLog->addHandler(std::shared_ptr<Handler>(new File_Handler(infoPath)));
    warningLog->addHandler(std::shared_ptr<Handler>(new File_Handler(warningPath)));
    errorLog->addHandler(std::shared_ptr<Handler>(new File_Handler(errorPath)));
}

template <typename Logger_Type>
Log_In_File<Logger_Type>::Log_In_File(const Log_In_File& cpy)
{}

template <typename Logger_Type>
Log_In_File<Logger_Type>& Log_In_File<Logger_Type>::operator = (const Log_In_File<Logger_Type>& noCpy)
{return *instance;}


#endif
