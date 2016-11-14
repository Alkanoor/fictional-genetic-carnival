#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP


#include <functional>


///************************************************************
/// Class that provides a pool to manipulate multiple threads
///************************************************************

class Thread_Pool
{
    public:
        Thread_Pool(const std::vector<int>& id_threads);

        size_t size() const;

        void add_to_thread(int id_in_pool, const std::function<void()>& to_exec);
        void add_to_thread_and_exec(int id_in_pool, const std::function<void()>& to_exec);

        const Thread& get_thread(int id_in_pool);

    private:
        std::vector<int> id_threads;
};


#endif
