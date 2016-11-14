#ifndef THREAD_HPP
#define THREAD_HPP


#include <thread>
#include <functional>


///****************************************************************
/// Class that provides a thread model based on standard threads
/// No copy of a thread is allowed
///****************************************************************

class Thread
{
    public:
        Thread_Pool(bool autostart = true);

        bool start();
        void restart();
        void stop();
        void terminate();

        bool is_started();

        void add_to_thread(int id, const std::function<void()>& to_exec);
        void add_to_thread_and_exec(int id, const std::function<void()>& to_exec);

        static const Thread& get_thread(int id);

    private:
        static std::map<int, Thread> threads;
};


#endif
