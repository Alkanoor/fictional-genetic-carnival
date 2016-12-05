#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP


#include "thread.hpp"


///************************************************************
/// Class that provides a pool to manipulate multiple threads
///************************************************************

class Thread_Pool
{
    public:
        Thread_Pool(const std::vector<int>& id_threads = std::vector<int>());

        size_t size() const;

        void append(int id);
        void erase(int id);

        void add_to_thread(int id_in_pool, const std::function<void()>& to_exec) const;
        void add_to_thread_and_exec(int id_in_pool, const std::function<void()>& to_exec) const;
        void wait_for(int id_in_pool) const;

        const Thread& get_thread(int id_in_pool);

    private:
        std::vector<int> id_threads;
};


#endif
