#include <cassert>
#include <algorithm>

#include "include/thread_pool.hpp"


Thread_Pool::Thread_Pool(const std::vector<int>& id_threads) :
    id_threads(id_threads)
{}

size_t Thread_Pool::size() const
{return id_threads.size();}

void Thread_Pool::append(int id)
{
    id_threads.push_back(id);
}

void Thread_Pool::erase(int id)
{
    id_threads.erase(std::find(id_threads.begin(), id_threads.end(), id));
}

void Thread_Pool::add_to_thread(int id_in_pool, const std::function<void()>& to_exec) const
{
    assert(id_in_pool<(int)id_threads.size());
    Thread::add_to_thread(id_threads[id_in_pool], to_exec);
}

void Thread_Pool::add_to_thread_and_exec(int id_in_pool, const std::function<void()>& to_exec) const
{
    assert(id_in_pool<(int)id_threads.size());
    Thread::add_to_thread_and_exec(id_threads[id_in_pool], to_exec);
}

void Thread_Pool::wait_for(int id_in_pool) const
{
    assert(id_in_pool<(int)id_threads.size());
    Thread::join(id_threads[id_in_pool]);
}

const Thread& Thread_Pool::get_thread(int id_in_pool)
{
    assert(id_in_pool<(int)id_threads.size());
    return Thread::get_thread(id_threads[id_in_pool]);
}
