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

    std::vector<Thread> t;
    //t.push_back(Thread(0)); // doesn't work because copy contructor is private : an id must be provided
    t.emplace_back(Thread(0));
}
