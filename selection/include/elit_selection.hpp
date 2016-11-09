#ifndef ELIT_SELECTION_HPP
#define ELIT_SELECTION_HPP


#include <memory>
#include <cassert>

#include "rank_selection.hpp"


///*******************************************************************************************************************************************************************
/// Class that implements an elit selection
///  N T N_threads   template parameters are the same as those from Selection class
/// The best "nb_to_keep" elements are kept according to their quality, other are selected with a selection that can be specified (rank selection by default)
///*******************************************************************************************************************************************************************

template <size_t N, typename T, size_t N_threads = 1>
class Elit_Selection : public Selection<N,T,N_threads>
{
    public:
        Elit_Selection(int nb_to_keep=1, std::shared_ptr<Selection<N,T,N_threads> > base=std::shared_ptr<Selection<N,T,N_threads> >(new Rank_Selection<N,T,T,N_threads>()));
        Elit_Selection(int thread_id, int nb_to_keep=1, std::shared_ptr<Selection<N,T,N_threads> > base=std::shared_ptr<Selection<N,T,N_threads> >(new Rank_Selection<N,T,T,N_threads>()));

        const std::array<int,N>& apply(const std::array<T,N>& qualities, int begin_at=0, bool already_sorted=false) throw ();

    private:
        int number_individuals_to_keep;
        std::shared_ptr<Selection<N,T,N_threads> > default_selection;
};


template <size_t N, typename T, size_t N_threads>
Elit_Selection<N,T,N_threads>::Elit_Selection(int nb_to_keep, std::shared_ptr<Selection<N,T,N_threads> > base) :
    Elit_Selection(0, nb_to_keep, base)
{}

template <size_t N, typename T, size_t N_threads>
Elit_Selection<N,T,N_threads>::Elit_Selection(int thread_id, int nb_to_keep, std::shared_ptr<Selection<N,T,N_threads> > base) :
    Selection<N,T,N_threads>(thread_id),
    number_individuals_to_keep(nb_to_keep),
    default_selection(base)
{
    assert(number_individuals_to_keep<=N);
}

template <size_t N, typename T, size_t N_threads>
const std::array<int,N>& Elit_Selection<N,T,N_threads>::apply(const std::array<T,N>& qualities, int begin_at, bool already_sorted) throw ()
{
    if(!already_sorted)
        Utils::index_after_sorting(qualities, begin_at, Selection<N,T,N_threads>::selected_sorted[Selection<N,T,N_threads>::thread_id], Selection<N,T,N_threads>::selected_sorted_reversed[Selection<N,T,N_threads>::thread_id]);
    const std::array<int,N>& temp = default_selection->apply(qualities, number_individuals_to_keep);
    for(int i=number_individuals_to_keep; i<N; i++)
    {
        Selection<N,T,N_threads>::selected_sorted[Selection<N,T,N_threads>::thread_id][i] = temp[i];
        Selection<N,T,N_threads>::selected_sorted_reversed[Selection<N,T,N_threads>::thread_id][temp[i]] = i;
    }
    return Selection<N,T,N_threads>::selected_sorted[Selection<N,T,N_threads>::thread_id];
}

#endif
