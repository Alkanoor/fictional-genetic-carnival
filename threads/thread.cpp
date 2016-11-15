#include "include/thread.hpp"

#include <stdexcept>


std::map<int, Thread&> Thread::threads;


Thread::Thread(int id, bool autostart, double sleep_between_instances, double sleep_between_operations, const std::function<void(double)>& sleep_function
                #ifdef LOG_EVENTS
                    , const std::shared_ptr<Info_Warning_Error_Logger>& info_logger
                #endif
                #ifdef LOG_EXCEPTIONS
                    , const std::shared_ptr<Info_Warning_Error_Logger>& error_logger
                #endif
               ) :
    thread_id(id),
    is_running(false),
    is_stopped(true),
    terminated(false),
    sleep_between_instances(sleep_between_instances),
    sleep_between_operations(sleep_between_operations),
    sleep_function(sleep_function)
    #ifdef LOG_EVENTS
        , events_logger(info_logger),
    #endif
    #ifdef LOG_EXCEPTIONS
        , error_logger(error_logger)
    #endif
{
    if(threads.count(id))
        throw std::runtime_error("Error: forbidden thread creation: same id as another thread");

    threads.emplace(id, *this);

    if(autostart)
        start();
}

bool Thread::start()
{
    if(!is_stopped)
    {
        #ifdef LOG_EVENTS
            events_logger->info("Starting thread ", thread_id, " not done : already started");
        #endif
        return false;
    }
    else
    {
        #ifdef LOG_EVENTS
            events_logger->info("Starting thread ", thread_id);
        #endif
    }

    is_stopped = false;
    is_running = true;
    thread = std::thread(&Thread::run);

    return true;
}

void Thread::restart()
{
    if(!terminated)
    {
        #ifdef LOG_EVENTS
            events_logger->info("Restarting thread ", thread_id);
        #endif
        if(is_stopped)
        {
            #ifdef LOG_EVENTS
                events_logger->info("(Was Stopped so new thread)");
            #endif
            thread = std::thread(&Thread::run);
        }
        is_stopped = false;
        is_running = true;
        mutex_on_to_exec.lock();
        to_exec.clear();
        mutex_on_to_exec.unlock();
    }
    else
    {
        #ifdef LOG_EVENTS
            events_logger->info("Restarting thread ", thread_id, " impossible because terminated was previously called");
        #endif
    }
}

void Thread::pause_processing()
{is_running = false;}

void Thread::continue_processing()
{is_running = true;}

void Thread::stop()
{
    #ifdef LOG_EVENTS
        events_logger->info("Stopping thread ", thread_id);
    #endif
    is_stopped = true;
    is_running = false;
    mutex_on_to_exec.lock();
    to_exec.clear();
    mutex_on_to_exec.unlock();
}

void Thread::soft_stop()
{
    #ifdef LOG_EVENTS
        events_logger->info("Stopping (soft) thread ", thread_id);
    #endif
    is_stopped = true;
    is_running = false;
}

void Thread::terminate()
{
    #ifdef LOG_EVENTS
        events_logger->info("Terminating thread ", thread_id);
    #endif
    terminated = true;
    thread.terminate();
}

void run()
{
    while(!is_stopped)
    {
        while(is_running)
        {
            mutex_on_to_exec.lock();
            while(to_exec.size())
            {
                std::function<void()> current_exec = *(to_exec.begin());
                to_exec.erase(to_exec.begin());
                mutex_on_to_exec.unlock();

                try
                {
                    current_exec();
                }
                catch(...)
                {
                    std::exception_ptr p = std::current_exception();
                    #ifdef LOG_EXCEPTION
                        if(logger)
                            logger->error("Exception in thread ", thread_id, " : ", (p ? p.__cxa_exception_type()->name() : "null"));
                    #endif
                }
                sleep_function(sleep_between_operations);

                mutex_on_to_exec.lock();
            }
            mutex_on_to_exec.unlock();

            sleep_function(sleep_between_operations);
        }
        sleep_function(sleep_between_instances);
    }
}

bool is_started() const
{return !is_stopped;}

void add_to_thread(int id, const std::function<void()>& to_exec)
{
    threads[id].mutex_on_to_exec.lock();
    threads[id].to_exec.push_back(to_exec);
    threads[id].mutex_on_to_exec.unlock();
}

void add_to_thread_and_exec(int id, const std::function<void()>& to_exec)
{
    threads[id].mutex_on_to_exec.lock();
    threads[id].to_exec.push_back(to_exec);
    threads[id].mutex_on_to_exec.unlock();

    if(is_stopped)
        restart();

}

static const Thread& get_thread(int id)
{
    return threads[id];
}


Thread::Thread()
{
    throw std::runtime_error("Error: forbidden operation: thread creation with no parameter");
}

Thread::Thread(const Thread&)
{
    throw std::runtime_error("Error: forbidden operation: copy of thread");
}

Thread& Thread::operator = (const Thread&)
{
    throw std::runtime_error("Error: forbidden operation: assignement of thread");
}
