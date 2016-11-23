#include "include/info_warning_error_logger_debug.hpp"


std::shared_ptr<Info_Warning_Error_Logger_Debug> Info_Warning_Error_Logger_Debug::static_log = std::shared_ptr<Info_Warning_Error_Logger_Debug>(new Info_Warning_Error_Logger_Debug());

Info_Warning_Error_Logger_Debug::Info_Warning_Error_Logger_Debug(bool debug, bool writeDebug, bool date, int type) :
    Info_Warning_Error_Logger(date, type),
    debug(debug),
    writeDebug(writeDebug)
{}

void Info_Warning_Error_Logger_Debug::toggleDebug()
{debug = !debug;}

void Info_Warning_Error_Logger_Debug::setDebug(bool debug)
{this->debug = debug;}

void Info_Warning_Error_Logger_Debug::setWriteDebug(bool writeDebug)
{this->writeDebug = writeDebug;}

bool Info_Warning_Error_Logger_Debug::isDebug() const
{return debug;}

bool Info_Warning_Error_Logger_Debug::isWriteDebug() const
{return writeDebug;}

void Info_Warning_Error_Logger_Debug::printDate()
{
    if(debug)
        for(std::vector<std::shared_ptr<Handler> >::iterator it = handlers.begin(); it != handlers.end(); it++)
            (**it)<<"<"<<date()<<"> ";
}

void Info_Warning_Error_Logger_Debug::endLine()
{
    if(debug)
        for(std::vector<std::shared_ptr<Handler> >::iterator it = handlers.begin(); it != handlers.end(); it++)
            (*it)->writeEndline("");
}

std::string Info_Warning_Error_Logger_Debug::type()
{return "debug";}
