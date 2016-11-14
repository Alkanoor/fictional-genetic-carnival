#include "thread_pool.hpp"


Thread_Pool::Thread_Pool(const std::vector<int>& id_threads) :
    id_threads(id_threads)
{}

size_t Thread_Pool::size() const
{return id_threads.size();}

void Thread_Pool::add_to_thread(int id_in_pool, const std::function<void()>& to_exec)
{
    assert(id_in_pool<(int)id_threads.size());
    Thread::add_to_thread(id_threads[id_in_pool], to_exec);
}

void Thread_Pool::add_to_thread_and_exec(int id_in_pool, const std::function<void()>& to_exec)
{
    assert(id_in_pool<(int)id_threads.size());
    Thread::add_to_thread_and_exec(id_threads[id_in_pool], to_exec);
}

const Thread& Thread_Pool::get_thread(int id_in_pool)
{
    assert(id_in_pool<(int)id_threads.size());
    return Thread::get_thread(id_threads[id_in_pool]);
}