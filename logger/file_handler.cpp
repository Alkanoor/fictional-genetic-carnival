#include "include/file_handler.hpp"


File_Handler::File_Handler(const std::string& path) :
    file(path.c_str(),std::ios::out|std::ios::trunc|std::ios::binary),
    path(path)
{}

File_Handler::~File_Handler()
{
    file.close();
    std::ifstream tmp(path.c_str(),std::ios::in|std::ios::binary);
    tmp.seekg(0, std::ios_base::beg);
    auto cur = tmp.tellg();
    tmp.seekg(0, std::ios_base::end);
    if(tmp.tellg()==cur)
        std::remove(path.c_str());
}

void File_Handler::write(const std::string& toWrite)
{
    if(file)
        file<<toWrite;
}

void File_Handler::writeEndline(const std::string& toWrite)
{
    if(file)
        file<<toWrite;
    if(file)
        file<<std::endl;
}

Handler& File_Handler::operator << (const std::string& toWrite)
{
    if(file)
        file<<toWrite;
    return *this;
}
