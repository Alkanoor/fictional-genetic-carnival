#ifndef MULTI_SELECTION_HPP
#define MULTI_SELECTION_HPP


//#define MULTI_SELECTION_DEBUG

#include <stdexcept>
#include <chrono>
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

        typedef Selection<N,T,N_threads> Selection_;
};


template <size_t M, size_t N, typename T, typename U, size_t N_threads>
std::array<std::array<int, N>, N_threads> Multi_Selection<M,N,T,U,N_threads>::chosen_selection;

template <size_t M, size_t N, typename T, typename U, size_t N_threads>
std::array<std::array<std::array<bool, N>, M>, N_threads> Multi_Selection<M,N,T,U,N_threads>::marked;

template <size_t M, size_t N, typename T, typename U, size_t N_threads>
std::default_random_engine Multi_Selection<M,N,T,U,N_threads>::random_engine(std::chrono::system_clock::now().time_since_epoch().count());

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
    #ifdef MULTI_SELECTION_DEBUG
        auto logger = Easy_Log_In_File_Debug::getInfoLog();
        logger->write("Applying on multi selection with ", qualities.size());
    #endif

    for(int j=0; j<(int)N; j++)
    {
        U random = distrib(random_engine);
        U sum = 0;
        int i = 0;
        while(i<(int)M && sum+selection_coeffs[i]<random)
        {
            sum += selection_coeffs[i];
            i++;
        }

        if(i>=(int)M)
            throw std::runtime_error("Error: bad value in mult selection, should never happen. Please slap the developper.");

        chosen_selection[Selection_::thread_id][j] = i;
        #ifdef MULTI_SELECTION_DEBUG
            logger->write("Chosen index for ", j, " : ", i);
        #endif
    }

    #ifdef MULTI_SELECTION_DEBUG
        logger->write("Giving ", Vector_To_String<const std::array<int, N> >(&chosen_selection[Selection_::thread_id]));
    #endif

    std::array<std::array<int,N>, M> temp_selected;
    std::array<std::array<int,N>, M> temp_selected_reversed;
    for(int i=0; i<(int)M; i++)
    {
        temp_selected[i] = selections[i]->apply(qualities, begin_at, already_sorted);
        temp_selected_reversed[i] = selections[i]->get_sorted_reversed();
        #ifdef MULTI_SELECTION_DEBUG
            logger->write("For i=", i, " we have ", Vector_To_String<const std::array<int, N> >(&temp_selected[i]));
            logger->write("And reversed : ", Vector_To_String<const std::array<int, N> >(&temp_selected_reversed[i]));
        #endif
    }

    std::array<int,M> mins;
    for(int i=0; i<(int)M; i++)
    {
        mins[i] = 0;
        for(int j=0; j<(int)N; j++)
            marked[Selection_::thread_id][i][j] = false;
    }

    for(int j=0; j<(int)N; j++)
    {
        int index = chosen_selection[Selection_::thread_id][j];
        int o = mins[index];
        while(o<(int)N && marked[Selection_::thread_id][index][o])
            o++;

        #ifdef MULTI_SELECTION_DEBUG
            logger->write("After analysis for j=", j, " we have index min o section[o] : ", index, " ", mins[index], " ", o, " => ", (temp_selected[index])[o]);
        #endif

        if(o>=(int)N)
            throw;

        Selection_::selected_sorted[Selection_::thread_id][j] = temp_selected[index][o];
        Selection_::selected_sorted_reversed[Selection_::thread_id][Selection_::selected_sorted[Selection_::thread_id][j]] = j;
        for(int i=0; i<(int)M; i++)
        {
            int p = temp_selected_reversed[i][temp_selected[index][o]];
            marked[Selection_::thread_id][i][p] = true;

            #ifdef MULTI_SELECTION_DEBUG
                logger->write("For i=", i, " we have selected[o] p : ", temp_selected[i][o], " ", p);
            #endif

            if(p==mins[i])
            {
                while(p<(int)N && marked[Selection_::thread_id][i][p])
                    p++;
                if(p<(int)N)
                    mins[i] = p;
            }

            #ifdef MULTI_SELECTION_DEBUG
                logger->write("New min ", i, " ", mins[i]);
            #endif
        }
    }

    #ifdef MULTI_SELECTION_DEBUG
        logger->endLine();
    #endif

    return Selection_::selected_sorted[Selection_::thread_id];
}


#endif
