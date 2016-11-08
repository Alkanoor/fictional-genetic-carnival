/**
Copyright (C)

creation date : 08/11/2015
file : easy_log_in_file.hpp
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


#ifndef EASY_LOG_IN_FILE_HPP
#define EASY_LOG_IN_FILE_HPP


#include "info_warning_error_logger.hpp"
#include "file_handler.hpp"


class Easy_Log_In_File
{
    public:
        static Easy_Log_In_File& getInstance();

        static std::shared_ptr<Info_Warning_Error_Logger> getInfoLog();
        static std::shared_ptr<Info_Warning_Error_Logger> getWarningLog();
        static std::shared_ptr<Info_Warning_Error_Logger> getErrorLog();

        static void setFolderPath(const std::string& path);

    private:
        Easy_Log_In_File(const std::string& folderPath);
        Easy_Log_In_File(const Easy_Log_In_File& cpy);
        Easy_Log_In_File& operator = (const Easy_Log_In_File& noCpy);

        static std::shared_ptr<Easy_Log_In_File> instance;

        std::shared_ptr<Info_Warning_Error_Logger> infoLog;
        std::shared_ptr<Info_Warning_Error_Logger> warningLog;
        std::shared_ptr<Info_Warning_Error_Logger> errorLog;

        std::string path;
        std::string infoPath;
        std::string warningPath;
        std::string errorPath;
};


#endif
