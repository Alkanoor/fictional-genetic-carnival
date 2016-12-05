#include "logger/include/log_in_file.hpp"
#include "util.hpp"

#include <mutex>


void Utils::terminate_and_close_logs(bool print_in_warnings)
{
    static std::mutex tmp;
    tmp.lock();
    if(print_in_warnings)
        Easy_Log_In_File::getWarningLog()->warning("Terminate thread, closing logs and exiting");
    Easy_Log_In_File& logger1 = Easy_Log_In_File::getInstance();
    logger1.close();
    if(print_in_warnings)
        Easy_Log_In_File_Debug::getWarningLog()->warning("Terminate thread, closing logs and exiting");
    Easy_Log_In_File_Debug& logger2 = Easy_Log_In_File_Debug::getInstance();
    logger2.close();
    tmp.unlock();
    exit(0);
}
