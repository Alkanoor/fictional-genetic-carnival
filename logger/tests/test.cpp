#include "../include/stdout_handler.hpp"
#include "../include/log_in_file.hpp"

/**
mkdir logs
mkdir logs/uselessDir
mkdir logs/uselessDirBis
g++ *.cpp -o test -std=c++11
./test
**/

int main()
{
    std::shared_ptr<Info_Warning_Error_Logger_Debug> logger = Easy_Log_In_File_Debug::getInfoLog();
    (*logger)<<"On commence la journée";
    logger->endLine();
    logger->info("On commence la journée");

    Easy_Log_In_File_Debug::setFolderPath("logs/uselessDir/");
    (*logger)<<"On commence la journée encore";
    logger->endLine();
    logger->warning("On commence la journée encore");
    logger = Easy_Log_In_File_Debug::getInfoLog();
    (*logger)<<"On commence la journée tu vas voir";
    logger->endLine();
    logger->error("On commence la journée tu vas voir");

    Easy_Log_In_File_Debug::setFolderPath("logs/uselessDirBis/");
    Easy_Log_In_File_Debug::setFolderPath("logs/");
    logger = Easy_Log_In_File_Debug::getWarningLog();

    (*logger)<<"Il est bien tard";
    logger->endLine();
    logger->write("Il est bien tard");

    std::shared_ptr<File_Handler> handler1(new File_Handler("logs/otherFile.txt"));
    logger->addHandler(handler1);

    (*logger)<<"Maintenant il n'est plus tard";
    logger->endLine();
    logger->warning("Maintenant il n'est plus tard");

    logger->addHandler(std::shared_ptr<Stdout_Handler>(new Stdout_Handler()));

    (*logger)<<"Maintenant la nuit est finie";
    logger->endLine();

    logger->setDebug(false);
    (*logger)<<"Maintenant Ca NE DEVRAIT PLUS rien écrire";
    logger->endLine();
    logger->endLine();
    logger->toggleDebug();
    logger->error("Maintenant Ca DEVRAIT écrire à nouveau");

    return 0;
}
