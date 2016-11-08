#include "include/easy_log_in_file.hpp"
#include "include/stdout_handler.hpp"

/**
mkdir logs
mkdir logs/uselessDir
mkdir logs/uselessDirBis
g++ *.cpp -o test -std=c++11
./test
**/

int main()
{
    std::shared_ptr<Info_Warning_Error_Logger> logger = Easy_Log_In_File::getInfoLog();
    (*logger)<<"On commence la journée";
    logger->endLine();

    Easy_Log_In_File::setFolderPath("logs/uselessDir/");
    (*logger)<<"On commence la journée encore";
    logger->endLine();
    logger = Easy_Log_In_File::getInfoLog();
    (*logger)<<"On commence la journée tu vas voir";
    logger->endLine();

    Easy_Log_In_File::setFolderPath("logs/uselessDirBis/");
    Easy_Log_In_File::setFolderPath("logs/");
    logger = Easy_Log_In_File::getWarningLog();

    (*logger)<<"Il est bien tard";
    logger->endLine();

    std::shared_ptr<File_Handler> handler1(new File_Handler("logs/otherFile.txt"));
    logger->addHandler(handler1);

    (*logger)<<"Maintenant il n'est plus tard";
    logger->endLine();

    logger->addHandler(std::shared_ptr<Stdout_Handler>(new Stdout_Handler()));

    (*logger)<<"Maintenant la nuit est finie";
    logger->endLine();

    return 0;
}
