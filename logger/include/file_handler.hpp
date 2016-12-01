/**
Copyright (C)

creation date : 05/11/2015
file : file_handler.hpp
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


#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP


#include <fstream>
#include <cstdio>

#include "handler.hpp"


class File_Handler : public Handler
{
    public:
        File_Handler(const std::string& path);
        ~File_Handler();

        void write(const std::string& toWrite);
        void writeEndline(const std::string& toWrite);
        Handler& operator << (const std::string& toWrite);

    private:
        std::ofstream file;
        std::string path;
};


#endif
