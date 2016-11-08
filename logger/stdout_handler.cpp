#include "include/stdout_handler.hpp"


Stdout_Handler::Stdout_Handler()
{}

Stdout_Handler::~Stdout_Handler()
{}

void Stdout_Handler::write(const std::string& toWrite)
{
    std::cout<<toWrite;
}

void Stdout_Handler::writeEndline(const std::string& toWrite)
{
    std::cout<<toWrite<<std::endl;
}

Handler& Stdout_Handler::operator << (const std::string& toWrite)
{
    std::cout<<toWrite;
    return *this;
}
