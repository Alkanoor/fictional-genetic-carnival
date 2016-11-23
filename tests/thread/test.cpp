/* This test provides benchmark for thread testing
   It searches the optimal number of threads running at same time */

#include <chrono>
#include <iostream>

#include "threads/include/thread_pool.hpp"


int long_computation(int a)
{
    int res = 1;
    for(int i=0; i<10000; i++)
        res += ((res>>2)&a)+3*i+(a&(1<<i));
    return res;
}

int reference_block() //about 50ms execution time on current machine
{
    for(int j=0;j<1000/45*50.5;j++)
        long_computation(100);
    return 0;
}

void multiple_reference_blocks(int offset)
{
    for(int i=0;i<200;i+=offset)
        reference_block();
}

// g++ -std=c++11 -I../../include/utils -I../.. test.cpp -o test -L../../logger/bin -llog ../../threads/thread.cpp ../../threads/thread_pool.cpp -lpthread
int main()
{
    for(int i=0;i<5;i++)
    {
        auto start = std::chrono::system_clock::now();
        reference_block();
        auto end = std::chrono::system_clock::now();
        std::cout<<(end-start).count()/1000<<std::endl;
    }

    //std::vector<Thread> t(1); // doesn't work because default contructor is private : an id must be provided

    //std::vector<Thread> threads;
    //threads.emplace_back(Thread(0)); //doesn't work because no move constructor can be created (because of mutex) and no copy constructor is allowed

    std::vector<std::shared_ptr<Thread> > threads;
    for(int j=0;j<10;j++)
        threads.push_back(std::shared_ptr<Thread>(new Thread(j)));

    for(int i=1;i<10;i++)
    {
        auto start = std::chrono::system_clock::now();
        for(int j=0;j<i;j++)
            Thread::add_to_thread_and_exec(j, std::bind(&multiple_reference_blocks,i));

        for(auto t : threads)
            t->join();

        auto end = std::chrono::system_clock::now();
        std::cout<<"With "<<i<<" threads : "<<(end-start).count()/1000<<std::endl;
        //exit(0);
    }
}
