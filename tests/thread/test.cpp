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
    for(int j=0; j<1000/45*50.5; j++)
        long_computation(100);
    return 0;
}

void multiple_reference_blocks(int offset)
{
    for(int i=0; i<100; i+=offset)
        reference_block();
}

// g++ -std=c++11 -I../.. test.cpp -o test -L../../logger/bin -llog ../../threads/thread.cpp ../../threads/thread_pool.cpp -lpthread -Wall -Werror
// if it doesn't work because of -llog, test with :
// g++ -std=c++11 -I../.. test.cpp -o test ../../threads/thread.cpp ../../threads/thread_pool.cpp $(echo $(ls ../../logger/*.cpp)) -lpthread -Wall -Werror
int main()
{
    for(int i=0; i<5; i++)
    {
        auto start = std::chrono::system_clock::now();
        reference_block();
        auto end = std::chrono::system_clock::now();
        std::cout<<"Testing reference duration : "<<(end-start).count()/1000<<" microseconds"<<std::endl;
    }

    //std::vector<Thread> t(1); // doesn't work because default contructor is private : an id must be provided

    //std::vector<Thread> threads;
    //threads.emplace_back(Thread(0)); //doesn't work because no move constructor can be created (because of mutex) and no copy constructor is allowed

    std::vector<std::shared_ptr<Thread> > threads;
    for(int j=0;j<10;j++)
        threads.push_back(std::shared_ptr<Thread>(new Thread(j)));

    std::vector<int> durations;
    for(int i=1; i<10; i++)
    {
        auto start = std::chrono::system_clock::now();
        for(int j=0; j<i; j++)
            for(int k=0; k<2; k++)
                Thread::add_to_thread_and_exec(j, std::bind(&multiple_reference_blocks,i));

        for(auto t : threads)
            t->join();

        auto end = std::chrono::system_clock::now();
        std::cout<<"With "<<i<<" threads : "<<(end-start).count()/1000<<" microseconds"<<std::endl;
        durations.push_back((end-start).count()/1000);
    }

    int best_n_threads = 0;
    for(int i=1; i<(int)durations.size(); i++)
    {
        //durations[i-1] = n/(n-1)*durations[i] (ideal)
        //durations[i-1]-durations[i] = 1/(n-1)*durations[i]
        int slope = durations[i-1]-durations[i];
        if(slope*i*1.5 <= durations[i] || slope < 0)
        {
            best_n_threads = i-1;
            break;
        }
    }

    std::cout<<"Best number threads (including main) found : "<<(best_n_threads+1)<<std::endl;

    return 0;
}
