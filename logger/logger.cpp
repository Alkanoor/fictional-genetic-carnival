#include "include/logger.hpp"


Logger::Logger()
{}

Logger::Logger(const std::vector<std::shared_ptr<Handler> >& handlers)
{
    this->handlers = handlers;
}

void Logger::addHandler(const std::shared_ptr<Handler>& handler)
{
    handlers.push_back(handler);
}

std::string Logger::date()
{
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    std::string ret = asctime(timeinfo);
    ret.resize(ret.size()-1);
    return ret;
}
