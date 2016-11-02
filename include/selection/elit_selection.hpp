#include "rank_selection.hpp"


template <size_t N, typename T, size_t N_threads = 1>
class Elit_Selection : public Selection<N,T,N_threads>
{
    public:
        Elit_Selection(int nb_to_keep=1, std::shared_ptr<Selection<N,T> > base=std::shared_ptr<Selection<N,T> >(new Rank_Selection<N,T>()));
        Elit_Selection(int thread_id, int nb_to_keep=1, std::shared_ptr<Selection<N,T> > base=std::shared_ptr<Selection<N,T> >(new Rank_Selection<N,T>()));

        const std::array<int,N>& apply(const std::array<T,N>& qualities, int begin_at=0);

    private:
        int number_individuals_to_keep;
        std::shared_ptr<Selection<N,T> > default_selection;
};


template <size_t N, typename T, size_t N_threads>
Elit_Selection<N,T,N_threads>::Elit_Selection(int nb_to_keep, std::shared_ptr<Selection<N,T> > base) :
    Elit_Selection(0, nb_to_keep, base)
{}

template <size_t N, typename T, size_t N_threads>
Elit_Selection<N,T,N_threads>::Elit_Selection(int thread_id, int nb_to_keep, std::shared_ptr<Selection<N,T> > base) :
    Selection<N,T,N_threads>(thread_id),
    number_individuals_to_keep(nb_to_keep),
    default_selection(base)
{
    assert(number_individuals_to_keep<=N);
}

template <size_t N, typename T, size_t N_threads>
const std::array<int,N>& Elit_Selection<N,T,N_threads>::apply(const std::array<T,N>& qualities, int begin_at)
{
    index_after_sorting(qualities, begin_at, selected_sorted[thread_id], selected_sorted_reversed[thread_id]);
    const std::array<int,N>& temp = default_selection->apply(qualities, number_individuals_to_keep);
    for(int i=number_individuals_to_keep; i<N; i++)
    {
        selected_sorted[thread_id][i] = temp[i];
        selected_sorted_reversed[thread_id][temp[i]] = i;
    }
    return selected_sorted[thread_id];
}
