#include "utils/util.hpp"
#include "include/thread.hpp"
#include "include/thread_pool.hpp"

#include <stdexcept>


std::map<int, Thread*> Thread::threads;
#ifdef ADD_TO_DEFAULT_POOL
    Thread_Pool Thread::default_pool;
#endif

#include <iostream>
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
    is_finished(true),
    dont_stop(false),
    stop_when_task_finished(false),
    sleep_between_instances(sleep_between_instances),
    sleep_between_operations(sleep_between_operations),
    sleep_function(sleep_function)
    #ifdef LOG_EVENTS
        , events_logger(info_logger)
    #endif
    #ifdef LOG_EXCEPTIONS
        , error_logger(error_logger)
    #endif
{
    if(threads.count(id))
        throw std::runtime_error("Error: forbidden thread creation: same id as another thread");

    threads.emplace(id, this);
    #ifdef ADD_TO_DEFAULT_POOL
        default_pool.append(id);
    #endif

    if(autostart)
        start();
}

Thread::~Thread()
{
    #ifdef ADD_TO_DEFAULT_POOL
        default_pool.erase(thread_id);
    #endif
}


bool Thread::start()
{
    std::cout<<"Lock mutex on finished in start "<<thread_id<<std::endl;
    mutex_on_finished.lock();
    if(!is_finished)
    {
        #ifdef LOG_EVENTS
            events_logger->info("Starting thread ", thread_id, " not done : already started");
        #endif
        mutex_on_finished.unlock();
        std::cout<<"Unlock mutex on finished in start"<<std::endl;
        return false;
    }
    else
    {
        #ifdef LOG_EVENTS
            events_logger->info("Starting thread ", thread_id);
        #endif
    }
    mutex_on_finished.unlock();
    std::cout<<"UNLock2 mutex on finished in start "<<thread_id<<std::endl;

    is_stopped = false;
    is_running = true;
    is_finished = false;
    dont_stop = false;
    thread = std::thread(std::bind(&Thread::run, this));

    return true;
}
#include <iostream>
void Thread::restart()
{
    std::cout<<"RESSTARTEU "<<thread_id<<std::endl;
    if(!terminated)
    {
        std::cout<<"Lock mutex on finished in restart "<<thread_id<<std::endl;
        mutex_on_finished.lock();

        #ifdef LOG_EVENTS
            events_logger->info("Restarting thread ", thread_id);
        #endif

        is_stopped = false;
        is_running = true;
        std::cout<<"Next step"<<std::endl;

        if(!is_finished)
            dont_stop = true;
        else
        {
            #ifdef LOG_EVENTS
                events_logger->info("(Was Stopped so new thread)");
            #endif

            dont_stop = false;
            is_finished = false;
            std::cout<<"Ne< thread"<<std::endl;
            thread = std::thread(std::bind(&Thread::run, this));
        }

        mutex_on_finished.unlock();
        std::cout<<"UNLock mutex on finished in restart"<<std::endl;
    }
    else
    {
        #ifdef LOG_EVENTS
            events_logger->info("Restarting thread ", thread_id, " impossible because terminated was previously called");
        #endif
    }
}

void Thread::pause_processing()
{
    is_running = false;
    #ifdef LOG_EVENTS
        events_logger->info("Pausing thread ", thread_id);
    #endif
}

void Thread::continue_processing()
{
    is_running = true;
    #ifdef LOG_EVENTS
        events_logger->info("Continuing thread ", thread_id);
    #endif
}

void Thread::stop()
{
    std::cout<<"Lock mutex on finished in stop"<<std::endl;
    mutex_on_finished.lock();

    if(dont_stop)
    {
        dont_stop = false;
        mutex_on_finished.unlock();
        std::cout<<"UNLock mutex on finished in stop"<<std::endl;
        return;
    }

    #ifdef LOG_EVENTS
        events_logger->info("Stopping thread ", thread_id);
    #endif

    is_stopped = true;
    is_running = false;
    std::cout<<"Lock mutex exec stop"<<std::endl;
    mutex_on_to_exec.lock();
    std::cout<<"CLLLEARRRING stop "<<thread_id<<std::endl;
    to_exec.clear();
    mutex_on_to_exec.unlock();
    std::cout<<"UNLock mutex exec stop "<<std::endl;

    mutex_on_finished.unlock();
    std::cout<<"UNLock mutex on finished in stop"<<std::endl;
}

void Thread::soft_stop()
{
    std::cout<<"Lock mutex on finished in softstop"<<std::endl;
    mutex_on_finished.lock();

    if(dont_stop)
    {
        dont_stop = false;
        mutex_on_finished.unlock();
        std::cout<<"UNLock mutex on finished in softstop"<<std::endl;
        return;
    }

    #ifdef LOG_EVENTS
        events_logger->info("Stopping (soft) thread ", thread_id);
    #endif

    is_stopped = true;
    is_running = false;

    mutex_on_finished.unlock();
    std::cout<<"UNLock mutex on finished in softstop"<<std::endl;
}

void Thread::terminate()
{
    #ifdef LOG_EVENTS
        events_logger->info("Terminating thread ", thread_id);
    #endif
    terminated = true;
    Utils::terminate_and_close_logs();
}

void Thread::join()
{
    mutex_on_to_exec.lock();
    while(to_exec.size())
    {
        mutex_on_to_exec.unlock();
        sleep_function(sleep_between_instances);
        mutex_on_to_exec.lock();
    }
    mutex_on_to_exec.unlock();

    is_stopped = true;
    is_running = false;
    std::cout<<"ATTEMPT TO JOIN"<<std::endl;
    Easy_Log_In_File_Threaded_Debug::getInstance()->write("oooo");
    if(thread.joinable())
        thread.join();
    std::cout<<"JOINED "<<thread_id<<std::endl;
}

void Thread::run()
{
    #ifdef LOG_EVENTS
        bool processing = false;
    #endif

    std::cout<<"Lock mutex on finished in run"<<std::endl;
    mutex_on_finished.lock();
    while(!is_stopped)
    {
        mutex_on_finished.unlock();
        std::cout<<"UNLock mutex on finished in run"<<std::endl;
        if(thread_id==0)
            std::cout<<"stopped "<<thread_id<<" => "<<is_stopped<<std::endl;
        while(is_running)
        {
            if(thread_id==0)
                std::cout<<"Running "<<thread_id<<" => "<<is_running<<std::endl;
            mutex_on_to_exec.lock();
            #ifdef LOG_EVENTS
                processing = to_exec.size()>0;
            #endif
            while(to_exec.size())
            {
                std::cout<<"================================================= popping "<<to_exec.size()<<" in "<<thread_id<<std::endl;
                std::function<void()> current_exec = *(to_exec.begin());
                to_exec.erase(to_exec.begin());
                mutex_on_to_exec.unlock();
                std::cout<<"UNLock mutex exec run"<<std::endl;

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

            #ifdef LOG_EVENTS
                if(processing)
                    events_logger->info("Finished processing in thread ", thread_id);
            #endif

            sleep_function(sleep_between_operations);

            if(stop_when_task_finished && !to_exec.size())
            {
                is_running = false;
                is_stopped = true;
                stop_when_task_finished = false;
            }
        }

        #ifdef LOG_EVENTS
            mutex_on_to_exec.lock();
            if(to_exec.size())
                events_logger->info("Pause processing in thread ", thread_id);
            mutex_on_to_exec.unlock();
        #endif

        sleep_function(sleep_between_instances);

        std::cout<<"Lock2 mutex on finished in run"<<std::endl;
        mutex_on_finished.lock();
    }

    std::cout<<"That was end but it seems mutex is activated"<<std::endl;
    mutex_on_to_exec.lock();
    std::cout<<"CLLLEARRRING "<<thread_id<<std::endl;
    to_exec.clear();
    mutex_on_to_exec.unlock();

    is_finished = true;
    mutex_on_finished.unlock();
    std::cout<<"UNLock2 mutex on finished in run"<<std::endl;
}

bool Thread::is_started() const
{return !is_stopped;}

void Thread::add_to_thread(int id, const std::function<void()>& to_exec)
{
    threads[id]->mutex_on_to_exec.lock();
    threads[id]->to_exec.push_back(to_exec);
    threads[id]->mutex_on_to_exec.unlock();

    #ifdef LOG_EVENTS
        threads[id]->events_logger->info("Single operation added to thread ", threads[id]->thread_id);
    #endif
}

void Thread::add_to_thread_and_exec(int id, const std::function<void()>& to_exec)
{
    std::cout<<"Lock mutex exec "<<id<<std::endl;
    threads[id]->mutex_on_to_exec.lock();
    threads[id]->to_exec.push_back(to_exec);
    threads[id]->mutex_on_to_exec.unlock();
    std::cout<<"UNLock mutex exec "<<id<<std::endl;

    std::cout<<"BREAKING NEWS in thread "<<threads[id]->thread_id<<" "<<id<<" "<<threads[id]->is_stopped<<" "<<threads[id]->is_running<<std::endl;

    #ifdef LOG_EVENTS
        threads[id]->events_logger->info("Single operation added to thread ", threads[id]->thread_id, ", immediate execution");
    #endif

    threads[id]->stop_when_task_finished = true;
    std::cout<<"Before lock in add to hread : "<<threads[id]->is_finished<<std::endl;
    threads[id]->mutex_on_finished.lock();
    std::cout<<"Inside lock before restart "<<std::endl;
    if(threads[id]->is_finished)
    {
        threads[id]->mutex_on_finished.unlock();
        threads[id]->restart();
    }
    else
    {
        std::cout<<"Spassici no restart"<<std::endl;
        threads[id]->is_stopped = false;
        threads[id]->is_running = true;
        threads[id]->mutex_on_finished.unlock();
    }
    std::cout<<"After lock "<<threads[id]->to_exec.size()<<std::endl;

    std::cout<<"BREAKING NEWS BIS in thread "<<threads[id]->thread_id<<" "<<id<<" "<<threads[id]->is_stopped<<" "<<threads[id]->is_running<<std::endl;
}

const Thread& Thread::get_thread(int id)
{
    return *threads[id];
}

#ifdef ADD_TO_DEFAULT_POOL
    const Thread_Pool& Thread::get_default_pool()
    {
        return default_pool;
    }
#endif


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
