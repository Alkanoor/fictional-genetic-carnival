#include "include/file_handler.hpp"
#include <sstream>


File_Handler::File_Handler(const std::string& path)
{
    int ctr = 0;
    std::string tmp = path;
    std::ifstream test(tmp.c_str(), std::ios::in|std::ios::binary);
    while(test)
    {
        ctr += 1;
        std::ostringstream oss;
        oss<<ctr;
        tmp = path+"_"+oss.str();
        test.open(tmp.c_str(), std::ios::in|std::ios::binary);
    }
    this->path = tmp;
    file.open(this->path.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
}

File_Handler::~File_Handler()
{
    file.close();
    std::ifstream tmp(path.c_str(), std::ios::in|std::ios::binary);
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
