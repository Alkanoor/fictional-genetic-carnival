#include "include/info_warning_error_logger_threaded_debug.hpp"


std::shared_ptr<Info_Warning_Error_Logger_Threaded_Debug> Info_Warning_Error_Logger_Threaded_Debug::static_log = std::shared_ptr<Info_Warning_Error_Logger_Threaded_Debug>(new Info_Warning_Error_Logger_Threaded_Debug());

Info_Warning_Error_Logger_Threaded_Debug::Info_Warning_Error_Logger_Threaded_Debug(bool debug, bool writeDebug, bool date, int type) :
    Info_Warning_Error_Logger_Debug(debug, writeDebug, date, type)
{}

void Info_Warning_Error_Logger_Threaded_Debug::printDate()
{
    mutex.lock();
    ((Info_Warning_Error_Logger_Debug*)this)->printDate();
    mutex.unlock();
}

void Info_Warning_Error_Logger_Threaded_Debug::endLine()
{
    mutex.lock();
    ((Info_Warning_Error_Logger_Debug*)this)->endLine();
    mutex.unlock();
}

std::string Info_Warning_Error_Logger_Threaded_Debug::type()
{return "debug_threaded";}
