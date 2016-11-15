#ifndef THREAD_HPP
#define THREAD_HPP


#define LOG_EXCEPTIONS
#define LOG_EVENTS


#include <thread>
#include <functional>

#include "logger/include/log_in_file.hpp"


///****************************************************************
/// Class that provides a thread model based on standard threads
/// No copy of a thread is allowed
///****************************************************************

class Thread
{
    public:
        Thread(int id, bool autostart = true, double sleep_between_instance = 0.1, double sleep_between_operations = 0.05, const std::function<void(double)>& sleep_function = std::bind(sleep,std::placeholders::_1)
                #ifdef LOG_EVENTS
                    , const std::shared_ptr<Info_Warning_Error_Logger>& info_logger = Easy_Log_In_File::getInfoLog()
                #endif
                #ifdef LOG_EXCEPTIONS
                    , const std::shared_ptr<Info_Warning_Error_Logger>& error_logger = Easy_Log_In_File::getErrorLog()
                #endif
               );

        bool start();               // launches the first part of the thread, is is_running, does nothing
        void restart();             // waits until the end of current operation but erase all remaining in queue, and restart cleanly
        void pause_processing();    // pauses processing of operations
        void continue_processing(); // continues processing of operations at the point it was before pausing
        void stop();                // waits until the end of current operation but erase all remaining in queue, and finish
        void soft_stop();           // waits until the end of all operations in queue, and finish
        void terminate();           // aborts current operations abruptely and quit as soon as it is possible

        void run();                 // main loop in thread
        bool is_started() const;

        void add_to_thread(int id, const std::function<void()>& to_exec);
        void add_to_thread_and_exec(int id, const std::function<void()>& to_exec);

        static const Thread& get_thread(int id);

    private:
        Thread();
        Thread(const Thread&);
        Thread& operator = (const Thread&);

        int thread_id;

        bool is_running;
        bool is_stopped;
        bool terminated;

        double sleep_between_instances;
        double sleep_between_operations;
        std::function<void(double)> sleep_function;

        std::mutex mutex_on_to_exec;
        std::vector<std::function<void()> > to_exec;

        #ifdef LOG_EVENTS
            std::shared_ptr<Info_Warning_Error_Logger> events_logger;
        #endif
        #ifdef LOG_EXCEPTIONS
            std::shared_ptr<Info_Warning_Error_Logger> error_logger;
        #endif

        static std::map<int, Thread&> threads;
};


#endif
