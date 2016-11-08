#include "include/easy_log_in_file.hpp"


std::shared_ptr<Easy_Log_In_File> Easy_Log_In_File::instance = std::shared_ptr<Easy_Log_In_File>(new Easy_Log_In_File("logs/"));

Easy_Log_In_File& Easy_Log_In_File::getInstance()
{return *instance;}

std::shared_ptr<Info_Warning_Error_Logger> Easy_Log_In_File::getInfoLog()
{return instance->infoLog;}

std::shared_ptr<Info_Warning_Error_Logger> Easy_Log_In_File::getWarningLog()
{return instance->warningLog;}

std::shared_ptr<Info_Warning_Error_Logger> Easy_Log_In_File::getErrorLog()
{return instance->errorLog;}

void Easy_Log_In_File::setFolderPath(const std::string& path)
{
    if(path!=instance->path)
        instance = std::shared_ptr<Easy_Log_In_File>(new Easy_Log_In_File(path));
}

Easy_Log_In_File::Easy_Log_In_File(const std::string& folderPath) :
    infoLog(std::shared_ptr<Info_Warning_Error_Logger>(new Info_Warning_Error_Logger())),
    warningLog(std::shared_ptr<Info_Warning_Error_Logger>(new Info_Warning_Error_Logger())),
    errorLog(std::shared_ptr<Info_Warning_Error_Logger>(new Info_Warning_Error_Logger()))
{
    path = folderPath;
    infoPath = folderPath+Logger::date()+".infoLog";
    warningPath = folderPath+Logger::date()+".warningLog";
    errorPath = folderPath+Logger::date()+".errorLog";
    infoLog->addHandler(std::shared_ptr<Handler>(new File_Handler(infoPath)));
    warningLog->addHandler(std::shared_ptr<Handler>(new File_Handler(warningPath)));
    errorLog->addHandler(std::shared_ptr<Handler>(new File_Handler(errorPath)));
}

Easy_Log_In_File::Easy_Log_In_File(const Easy_Log_In_File& cpy)
{}

Easy_Log_In_File& Easy_Log_In_File::operator = (const Easy_Log_In_File& noCpy)
{return *instance;}
