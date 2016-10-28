#include "selection.hpp"


template <size_t N, typename T, size_t N_threads = 1>
class Quality_Selection : public Selection<N,T,N_threads>
{
    public:
        Quality_Selection(int thread_id=0);

        const std::array<N,int>& apply(const std::array<N,T>& qualities, int begin_at=0);
};


template <size_t N, typename T, size_t N_threads>
Quality_Selection<N,T,N_threads>::Quality_Selection(int id) :
    Selection<N,T,N_threads>(id)
{}
