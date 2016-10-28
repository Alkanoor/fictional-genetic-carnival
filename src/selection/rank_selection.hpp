#include "selection.hpp"


template <size_t N, typename T, size_t N_threads = 1>
class Rank_Selection : public Selection<N,T,N_threads>
{
    public:
        Rank_Selection(int thread_id=0);

        const std::array<N,int>& apply(const std::array<N,T>& qualities, int begin_at=0);

    private:
        static std::array<Quality_Selection<N, int>, N_threads+1> rank_selection;
};


template <size_t N, typename T, size_t N_threads>
Rank_Selection<N,T,N_threads>::Rank_Selection(int id) :
    Selection<N,T,N_threads>(id)
{}

template <size_t N, typename T, size_t N_threads>
const std::array<N,int>& Rank_Selection<N,T,N_threads>::apply(const std::array<N,T>& qualities, int begin_at)
{
    index_after_sorting(qualities, begin_at, selected_sorted[thread_id], selected_sorted_reversed[thread_id]);
    selected_sorted[thread_id] = rank_selection[thread_id].apply(selected_sorted_reversed[thread_id], begin_at);
    selected_sorted_reversed[thread_id] = rank_selection.get_sorted_reversed();
    return selected_sorted[thread_id];
}
