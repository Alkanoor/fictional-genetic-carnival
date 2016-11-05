#ifndef RANK_SELECTION_HPP
#define RANK_SELECTION_HPP


#include "selection.hpp"
#include "../utils/util.hpp"


///*********************************************************************************
/// Class that implements a rank selection (using a quality selection)
///  N T N_threads   template parameters are the same as those from Selection class
///  U               is the type on which quality selection on ranks is done
/// A functor is applied on ranks, then a quality selection is applied on
///*********************************************************************************

template <size_t N, typename T, typename U, size_t N_threads = 1>
class Rank_Selection : public Selection<N,T,N_threads>
{
    public:
        Rank_Selection(int thread_id=0, const std::function<U(int, int)>& functor=std::bind(&Rank_Selection<N,T,N_threads>::basic_conversion,U(2)));

        const std::array<int, N>& apply(const std::array<T, N>& qualities, int begin_at=0, bool already_sorted=false) throw ();

        static U basic_conversion(U offset, int rank, int size);

    private:
        std::function<U(int, int)> functor_to_be_applied_on_ranks;
        static std::array<Quality_Selection<N, int, N_threads>, N_threads> rank_selection;
        static std::array<std::array<U, N>, N_threads> selected_sorted_reversed_modified;
};


template <size_t N, typename T, typename U, size_t N_threads>
std::array<Quality_Selection<N, int>, N_threads> Rank_Selection<N,T,U,N_threads>::rank_selection;

template <size_t N, typename T, typename U, size_t N_threads>
std::array<std::array<U, N>, N_threads> Rank_Selection<N,T,U,N_threads>::selected_sorted_reversed_modified;


template <size_t N, typename T, typename U, size_t N_threads>
Rank_Selection<N,T,U,N_threads>::Rank_Selection(int id, const std::function<U(int, int)>& functor) :
    Selection<N,T,N_threads>(id),
    functor_to_be_applied_on_ranks(functor)
{}

template <size_t N, typename T, typename U, size_t N_threads>
const std::array<int, N>& Rank_Selection<N,T,U,N_threads>::apply(const std::array<T, N>& qualities, int begin_at, bool already_sorted) throw ()
{
    Utils::index_after_sorting(qualities, begin_at, selected_sorted[thread_id], selected_sorted_reversed_modified[thread_id], functor_to_be_applied_on_ranks);
    selected_sorted[thread_id] = rank_selection[thread_id].apply(selected_sorted_reversed_modified[thread_id], begin_at, true);
    selected_sorted_reversed[thread_id] = rank_selection.get_sorted_reversed();
    return selected_sorted[thread_id];
}

template <size_t N, typename T, typename U, size_t N_threads>
U Rank_Selection<N,T,U,N_threads>::basic_conversion(U offset, int rank, int size)
{
    return offset-U(rank)/U(size);
}

#endif
