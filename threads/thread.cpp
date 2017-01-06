#include "utils/util.hpp"
#include "include/thread.hpp"
#include "include/thread_pool.hpp"

#include <stdexcept>


std::map<int, Thread*> Thread::threads;
#ifdef ADD_TO_DEFAULT_POOL
    Thread_Pool Thread::default_pool;
#endif

#ifdef LOG_MUTEX_DEBUG
    std::shared_ptr<Info_Warning_Error_Logger_Threaded_Debug> Thread::debug_logger;
#endif


Thread::Thread(int id, bool autostart, double sleep_between_instances, double sleep_between_operations, const std::function<void(double)>& sleep_function
                #ifdef LOG_EVENTS
                    , const std::shared_ptr<Info_Warning_Error_Logger_Threaded>& info_logger
                #endif
                #ifdef LOG_EXCEPTIONS
                    , const std::shared_ptr<Info_Warning_Error_Logger_Threaded>& error_logger
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
    #ifdef LOG_MUTEX_DEBUG
        if(!debug_logger)
            debug_logger = Easy_Log_In_File_Threaded_Debug::getInstance().getInfoLog();
    #endif

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
    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Lock mutex on logic in start in thread ", thread_id);
    #endif
    mutex_on_finished.lock();
    if(!is_finished)
    {
        #ifdef LOG_EVENTS
            events_logger->info("Starting thread ", thread_id, " not done : already started");
        #endif
        mutex_on_finished.unlock();
        #ifdef LOG_MUTEX_DEBUG
            debug_logger->write("Unlock mutex on logic in start in thread ", thread_id);
        #endif

        return false;
    }
    else
    {
        #ifdef LOG_EVENTS
            events_logger->info("Starting thread ", thread_id);
        #endif
    }
    mutex_on_finished.unlock();
    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Unlock mutex on logic in start in thread ", thread_id);
    #endif

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
    if(!terminated)
    {
        #ifdef LOG_MUTEX_DEBUG
            debug_logger->write("Lock mutex on logic in restart in thread ", thread_id);
        #endif
        mutex_on_finished.lock();

        #ifdef LOG_EVENTS
            events_logger->info("Restarting thread ", thread_id);
        #endif

        is_stopped = false;
        is_running = true;

        if(!is_finished)
            dont_stop = true;
        else
        {
            #ifdef LOG_EVENTS
                events_logger->info("(Was Stopped so new thread)");
            #endif

            dont_stop = false;
            is_finished = false;
            #ifdef LOG_MUTEX_DEBUG
                debug_logger->write("New thread for id ", thread_id);
            #endif

            if(thread.joinable())
                thread.join();
            thread = std::thread(std::bind(&Thread::run, this));
        }

        mutex_on_finished.unlock();
        #ifdef LOG_MUTEX_DEBUG
            debug_logger->write("Unlock mutex on logic in restart in thread ", thread_id);
        #endif
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
    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Lock mutex on logic in stop in thread ", thread_id);
    #endif
    mutex_on_finished.lock();

    if(dont_stop)
    {
        dont_stop = false;
        mutex_on_finished.unlock();
        #ifdef LOG_MUTEX_DEBUG
            debug_logger->write("Unlock mutex on logic in stop in thread ", thread_id);
        #endif
        return;
    }

    #ifdef LOG_EVENTS
        events_logger->info("Stopping thread ", thread_id);
    #endif

    is_stopped = true;
    is_running = false;

    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Lock mutex on data in stop in thread ", thread_id);
    #endif

    mutex_on_to_exec.lock();
    to_exec.clear();
    mutex_on_to_exec.unlock();

    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Unlock mutex on data in stop in thread ", thread_id);
    #endif

    mutex_on_finished.unlock();
    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Unlock mutex on logic in stop in thread ", thread_id);
    #endif
}

void Thread::soft_stop()
{
    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Lock mutex on logic in soft stop in thread ", thread_id);
    #endif
    mutex_on_finished.lock();

    if(dont_stop)
    {
        dont_stop = false;
        mutex_on_finished.unlock();
        #ifdef LOG_MUTEX_DEBUG
            debug_logger->write("Unlock mutex on logic in soft stop in thread ", thread_id);
        #endif
        return;
    }

    #ifdef LOG_EVENTS
        events_logger->info("Stopping (soft) thread ", thread_id);
    #endif

    is_stopped = true;
    is_running = false;

    mutex_on_finished.unlock();
    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Unlock mutex on logic in soft stop in thread ", thread_id);
    #endif
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
    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Attempt to join ", thread_id);
    #endif
    if(thread.joinable())
        thread.join();
    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Thread ", thread_id, " joined");
    #endif
}

void Thread::run()
{
    #ifdef LOG_EVENTS
        bool processing = false;
    #endif

    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Lock mutex on logic in run in thread ", thread_id);
    #endif
    mutex_on_finished.lock();
    while(!is_stopped)
    {
        mutex_on_finished.unlock();
        #ifdef LOG_MUTEX_DEBUG
            debug_logger->write("Unlock mutex on logic in run in thread ", thread_id);
        #endif

        while(is_running)
        {
            if(to_exec.size())
            {
                #ifdef LOG_MUTEX_DEBUG
                    debug_logger->write("Lock mutex on data in run in thread ", thread_id);
                #endif

                #ifdef LOG_EVENTS
                    processing = true;
                #endif
            }
            mutex_on_to_exec.lock();
            while(to_exec.size())
            {
                #ifdef LOG_MUTEX_DEBUG
                    debug_logger->write("==== Popping action in a ", to_exec.size(), " sized table in thread ", thread_id);
                #endif

                std::function<void()> current_exec = *(to_exec.begin());
                to_exec.erase(to_exec.begin());
                mutex_on_to_exec.unlock();
                #ifdef LOG_MUTEX_DEBUG
                    debug_logger->write("Unlock mutex on data in run in thread ", thread_id);
                #endif

                try
                {
                    current_exec();
                }
                catch(...)
                {
                    std::exception_ptr p = std::current_exception();
                    #ifdef LOG_EXCEPTION
                        if(error_logger)
                            error_logger->error("Exception in thread ", thread_id, " : ", (p ? p.__cxa_exception_type()->name() : "null"));
                    #endif
                }
                sleep_function(sleep_between_operations);

                mutex_on_to_exec.lock();
            }
            mutex_on_to_exec.unlock();

            #ifdef LOG_EVENTS
                if(processing)
                {
                    processing = false;
                    events_logger->info("Finished processing in thread ", thread_id);
                }
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

        #ifdef LOG_MUTEX_DEBUG
            debug_logger->write("Lock mutex on logic in run in thread ", thread_id);
        #endif
        mutex_on_finished.lock();
    }

    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Lock mutex on data in run in thread ", thread_id);
    #endif

    mutex_on_to_exec.lock();
    to_exec.clear();
    mutex_on_to_exec.unlock();

    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Unlock mutex on data in run in thread ", thread_id, ", leaving");
    #endif

    is_finished = true;
    mutex_on_finished.unlock();

    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Unlock mutex on logic in run in thread ", thread_id, ", leaving");
    #endif
}

bool Thread::is_started() const
{return !is_stopped;}

void Thread::add_to_thread(int id, const std::function<void()>& to_exec)
{
    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Lock mutex on data in order added in thread ", id);
    #endif

    threads[id]->mutex_on_to_exec.lock();
    threads[id]->to_exec.push_back(to_exec);
    threads[id]->mutex_on_to_exec.unlock();

    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Unlock mutex on data in order added in thread ", id);
    #endif

    #ifdef LOG_EVENTS
        threads[id]->events_logger->info("Single operation added to thread ", threads[id]->thread_id);
    #endif
}

void Thread::add_to_thread_and_exec(int id, const std::function<void()>& to_exec)
{
    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Lock mutex on data in order added and executed in thread ", id);
    #endif

    threads[id]->mutex_on_to_exec.lock();
    threads[id]->to_exec.push_back(to_exec);
    threads[id]->mutex_on_to_exec.unlock();

    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Unlock mutex on data in order added and executed in thread ", id);
    #endif

    #ifdef LOG_EVENTS
        threads[id]->events_logger->info("Single operation added to thread ", threads[id]->thread_id, ", immediate execution");
    #endif

    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Lock mutex on logic in order added and executed in thread ", id);
    #endif

    threads[id]->stop_when_task_finished = true;
    threads[id]->mutex_on_finished.lock();
    if(threads[id]->is_finished)
    {
        threads[id]->mutex_on_finished.unlock();
        threads[id]->restart();
    }
    else
    {
        threads[id]->is_stopped = false;
        threads[id]->is_running = true;
        threads[id]->mutex_on_finished.unlock();
    }

    #ifdef LOG_MUTEX_DEBUG
        debug_logger->write("Unlock mutex on logic in order added and executed in thread ", id);
    #endif
}

const Thread& Thread::get_thread(int id)
{
    return *threads[id];
}

void Thread::join(int id)
{
    threads[id]->join();
}

#ifdef ADD_TO_DEFAULT_POOL
    const Thread_Pool& Thread::get_default_pool()
    {
        return default_pool;
    }

    Thread_Pool& Thread::get_and_modify_default_pool()
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
