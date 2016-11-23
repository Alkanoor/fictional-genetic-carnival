#include "include/info_warning_error_logger_threaded.hpp"


std::shared_ptr<Info_Warning_Error_Logger_Threaded> Info_Warning_Error_Logger_Threaded::static_log = std::shared_ptr<Info_Warning_Error_Logger_Threaded>(new Info_Warning_Error_Logger_Threaded());

Info_Warning_Error_Logger_Threaded::Info_Warning_Error_Logger_Threaded(bool date, int type) :
    Info_Warning_Error_Logger(date, type)
{}

void Info_Warning_Error_Logger_Threaded::printDate()
{
    mutex.lock();
    ((Info_Warning_Error_Logger*)this)->printDate();
    mutex.unlock();
}

void Info_Warning_Error_Logger_Threaded::endLine()
{
    mutex.lock();
    ((Info_Warning_Error_Logger*)this)->endLine();
    mutex.unlock();
}

std::string Info_Warning_Error_Logger_Threaded::type()
{return "threaded";}
