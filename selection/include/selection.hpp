#ifndef SELECTION_HPP
#define SELECTION_HPP


#include <cassert>
#include <array>


///******************************************************************************
/// Class that provides model for selection step during GA process
///  N         is the number of individuals in the population
///  T         is the type of quality which will be used to compare generations
///  N_threads is the number of threads that may be used simultaneously
///******************************************************************************

template <size_t N, typename T, size_t N_threads = 1>
class Selection
{
    public:
        Selection(int thread_id=0);

        //Pre condition :
        // - if already_sorted=false : none
        // - else selected_sorted must be set from at least begin_at before function is called
        virtual const std::array<int, N>& apply(const std::array<T, N>& qualities, int begin_at=0, bool already_sorted=false) throw () = 0;

        const std::array<int, N>& get_sorted()
        {return selected_sorted[thread_id];}

        const std::array<int, N>& get_sorted_reversed()
        {return selected_sorted_reversed[thread_id];}

        void set_selected_sorted(const std::array<int, N>& a)
        {selected_sorted[thread_id] = a;}

        void set_selected_sorted_reversed(const std::array<int, N>& a)
        {selected_sorted_reversed[thread_id] = a;}

    protected:
        int thread_id;

        static std::array<std::array<int, N>, N_threads> selected_sorted;
        static std::array<std::array<int, N>, N_threads> selected_sorted_reversed;
};


template <size_t N, typename T, size_t N_threads>
std::array<std::array<int, N>, N_threads> Selection<N,T,N_threads>::selected_sorted;

template <size_t N, typename T, size_t N_threads>
std::array<std::array<int, N>, N_threads> Selection<N,T,N_threads>::selected_sorted_reversed;


template <size_t N, typename T, size_t N_threads>
Selection<N,T,N_threads>::Selection(int id) :
    thread_id(id)
{
    assert(thread_id<(int)N_threads);
}


#endif
