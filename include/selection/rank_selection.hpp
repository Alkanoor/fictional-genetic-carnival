#include "selection.hpp"


///*********************************************************************************
/// Class that implements a rank selection (using a quality selection)
///  N T N_threads   template parameters are the same as those from Selection class
///  U               is the type on which quality selection on ranks is done
/// A functor is applied on ranks, then a quality selection is applied on
///*********************************************************************************

template <size_t N, typename T, size_t N_threads = 1>
class Rank_Selection : public Selection<N,T,N_threads>
{
    public:
        Rank_Selection(int thread_id=0);

        const std::array<int, N>& apply(const std::array<T, N>& qualities, int begin_at=0);

    private:
        std::function<U(int, int)> functor_to_be_applied_on_ranks;
        static std::array<Quality_Selection<N, int>, N_threads> rank_selection;
        static std::array<std::array<U, N>, N_threads> selected_sorted_reversed;
};


template <size_t N, typename T, size_t N_threads>
Rank_Selection<N,T,N_threads>::Rank_Selection(int id) :
    Selection<N,T,N_threads>(id)
{}

template <size_t N, typename T, size_t N_threads>
const std::array<int, N>& Rank_Selection<N,T,N_threads>::apply(const std::array<T, N>& qualities, int begin_at)
{
    index_after_sorting(qualities, begin_at, selected_sorted[thread_id], selected_sorted_reversed[thread_id], functor_to_be_applied_on_ranks);
    selected_sorted[thread_id] = rank_selection[thread_id].apply(selected_sorted_reversed[thread_id], begin_at);
    selected_sorted_reversed[thread_id] = rank_selection.get_sorted_reversed();
    return selected_sorted[thread_id];
}
