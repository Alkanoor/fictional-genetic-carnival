#ifndef MULTI_SELECTION_HPP
#define MULTI_SELECTION_HPP


#include <memory>
#include <random>

#include "selection.hpp"


///**********************************************************************************
/// Class that implements a multi selection
///  N T N_threads   template parameters are the same as those from Selection class
///  M               is the number of sub-selections used
///  U               is the type of coeffs that represent weight of each selection
/// A lottery on sub-selections is applied based on coeffs
///**********************************************************************************

template <size_t M, size_t N, typename T, typename U, size_t N_threads = 1>
class Multi_Selection : public Selection<N,T,N_threads>
{
    public:
        Multi_Selection(const std::array<std::shared_ptr<Selection<N,T> >, M>& selections, const std::array<U, M>& coeffs);
        Multi_Selection(int thread_id, const std::array<std::shared_ptr<Selection<N,T> >, M>& selections, const std::array<U, M>& coeffs);

        const std::array<int,N>& apply(const std::array<T,N>& qualities, int begin_at=0, bool already_sorted=false) throw ();

    private:
        std::array<std::shared_ptr<Selection<N, T, N_threads> >, M> selections;
        std::array<U, M> selection_coeffs;

        static std::array<std::array<int, N>, N_threads> chosen_selection;
        static std::array<std::array<std::array<bool, N>, M>, N_threads> marked;
        static std::default_random_engine random_engine;
        static std::uniform_real_distribution<U> distrib;
};


template <size_t M, size_t N, typename T, typename U, size_t N_threads>
std::array<std::array<int, N>, N_threads> Multi_Selection<M,N,T,U,N_threads>::chosen_selection;

template <size_t M, size_t N, typename T, typename U, size_t N_threads>
std::array<std::array<std::array<bool, N>, M>, N_threads> Multi_Selection<M,N,T,U,N_threads>::marked;

template <size_t M, size_t N, typename T, typename U, size_t N_threads>
std::default_random_engine Multi_Selection<M,N,T,U,N_threads>::random_engine;

template <size_t M, size_t N, typename T, typename U, size_t N_threads>
std::uniform_real_distribution<U> Multi_Selection<M,N,T,U,N_threads>::distrib(U(0),U(1));


template <size_t M, size_t N, typename T, typename U, size_t N_threads>
Multi_Selection<M,N,T,U,N_threads>::Multi_Selection(const std::array<std::shared_ptr<Selection<N,T> >, M>& selections, const std::array<U, M>& coeffs) :
    Multi_Selection<M,N,T,U,N_threads>(0, selections, coeffs)
{}

template <size_t M, size_t N, typename T, typename U, size_t N_threads>
Multi_Selection<M,N,T,U,N_threads>::Multi_Selection(int thread_id, const std::array<std::shared_ptr<Selection<N,T> >, M>& selections, const std::array<U, M>& coeffs) :
    Selection<N,T,N_threads>(thread_id),
    selections(selections),
    selection_coeffs(coeffs)
{}

template <size_t M, size_t N, typename T, typename U, size_t N_threads>
const std::array<int,N>& Multi_Selection<M,N,T,U,N_threads>::apply(const std::array<T,N>& qualities, int begin_at, bool already_sorted) throw ()
{
    for(int j=0; j<N; j++)
    {
        U random = distrib(random_engine);
        U sum = 0;
        int i = 0;
        while(i<M && sum+selection_coeffs[i]<random)
        {
            sum += selection_coeffs[i];
            i++;
        }

        if(i>=M)
            throw;

        chosen_selection[Selection<N,T,N_threads>::thread_id][j] = i;
    }

    std::array<const std::array<int,N>*, M> temp_selected;
    std::array<const std::array<int,N>*, M> temp_selected_reversed;
    for(int i=0; i<M; i++)
    {
        temp_selected[i] = selections[i]->apply_pointer(qualities, begin_at, already_sorted);
        temp_selected_reversed[i] = selections[i]->get_sorted_reversed_pointer();
    }

    std::array<int,M> mins;
    for(int i=0; i<M; i++)
    {
        mins[i] = 0;
        for(int j=0; j<N; j++)
            marked[Selection<N,T,N_threads>::thread_id][i][j] = false;
    }

    for(int j=0; j<N; j++)
    {
        int index = chosen_selection[Selection<N,T,N_threads>::thread_id][j];
        int o = mins[index];
        while(o<N && marked[Selection<N,T,N_threads>::thread_id][index][o])
            o++;

        if(o>=N)
            throw;

        Selection<N,T,N_threads>::selected_sorted[Selection<N,T,N_threads>::thread_id][j] = (*temp_selected[index])[o];
        Selection<N,T,N_threads>::selected_sorted_reversed[Selection<N,T,N_threads>::thread_id][Selection<N,T,N_threads>::selected_sorted[Selection<N,T,N_threads>::thread_id][j]] = j;
        marked[Selection<N,T,N_threads>::thread_id][index][o] = true;
        for(int i=0; i<M; i++)
        {
            int p = (*temp_selected_reversed[i])[(*temp_selected[i])[o]];
            marked[Selection<N,T,N_threads>::thread_id][i][p] = true;

            if(p==mins[i]+1)
            {
                while(p<N && marked[Selection<N,T,N_threads>::thread_id][i][p])
                    p++;
                if(p<N)
                    mins[i] = p;
            }
        }
    }

    return Selection<N,T,N_threads>::selected_sorted[Selection<N,T,N_threads>::thread_id];
}

#endif
