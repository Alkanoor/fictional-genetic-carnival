#ifndef QUALITY_SELECTION_HPP
#define QUALITY_SELECTION_HPP


//#define QUALITY_SELECTION_DEBUG

#include <stdexcept>
#include <chrono>
#include <random>

#include "logger/include/log_in_file.hpp"
#include "utils/vector_to_string.hpp"
#include "utils/util.hpp"
#include "selection.hpp"


///*********************************************************************************
/// Class that implements a quality selection
///  N T N_threads   template parameters are the same as those from Selection class
/// A lottery is applied on qualities in order to sort them with probabilities
///*********************************************************************************

template <size_t N, typename T, size_t N_threads = 1>
class Quality_Selection : public Selection<N,T,N_threads>
{
    public:
        Quality_Selection(int thread_id=0);

        const std::array<int, N>& apply(const std::array<T, N>& qualities, int begin_at=0, bool already_sorted=false) throw ();

    private:
        std::array<std::array<bool, N>, N_threads> marked;
        static std::default_random_engine random_engine;
        static std::uniform_real_distribution<T> distrib;

        typedef Selection<N,T,N_threads> Selection_;
};


template <size_t N, typename T, size_t N_threads>
std::default_random_engine Quality_Selection<N,T,N_threads>::random_engine(std::chrono::system_clock::now().time_since_epoch().count());

template <size_t N, typename T, size_t N_threads>
std::uniform_real_distribution<T> Quality_Selection<N,T,N_threads>::distrib(T(0),T(1));


template <size_t N, typename T, size_t N_threads>
Quality_Selection<N,T,N_threads>::Quality_Selection(int id) :
    Selection<N,T,N_threads>(id)
{}

template <size_t N, typename T, size_t N_threads>
const std::array<int, N>& Quality_Selection<N,T,N_threads>::apply(const std::array<T, N>& qualities, int begin_at, bool already_sorted) throw ()
{
    #ifdef QUALITY_SELECTION_DEBUG
        auto logger = Easy_Log_In_File_Debug::getInfoLog();
    #endif

    if(!already_sorted)
        Utils::index_after_sorting(qualities, begin_at, Selection_::selected_sorted[Selection_::thread_id], Selection_::selected_sorted_reversed[Selection_::thread_id]);

    T max_cumulated = 0;
    for(int i=begin_at; i<(int)N; i++)
    {
        marked[Selection_::thread_id][i] = false;
        max_cumulated += qualities[Selection_::selected_sorted[Selection_::thread_id][i]];
    }

    #ifdef QUALITY_SELECTION_DEBUG
        logger->write("Applying on quality selection with ", qualities.size());
        logger->write("Max cumulated : ", max_cumulated);
        logger->write(Vector_To_String<const std::array<T, N>&>(qualities));
        logger->write(Vector_To_String<const std::array<int, N>&>(Selection_::selected_sorted[Selection_::thread_id]));
        logger->write(Vector_To_String<const std::array<int, N>&>(Selection_::selected_sorted_reversed[Selection_::thread_id]));
        logger->endLine();
    #endif

    std::vector<int> indexes(N-begin_at);
    int j = 0, tmp = -1, min_offset = begin_at-1, index;
    for(int i=begin_at; i<(int)N; i++)
    {
        T random_01 = distrib(random_engine);
        T cumulated = 0;
        index = 0;
        tmp = -1;
        for(j=min_offset+1; j<(int)N; j++)
        {
            index = Selection_::selected_sorted[Selection_::thread_id][j];
            if(!marked[Selection_::thread_id][j])
            {
                tmp = j;
                #ifdef QUALITY_SELECTION_DEBUG
                    logger->write("Adding ", index, " => ", qualities[index]);
                #endif
                cumulated += qualities[index];
                if(cumulated>random_01*max_cumulated)
                    break;
            }
        }

        #ifdef QUALITY_SELECTION_DEBUG
            logger->write("Iteration ", i, " => random = ", random_01, " giving ", random_01*max_cumulated);
            logger->write("j index and cumulated obtained : ", j, " ", index, " ", cumulated);
            logger->write("Gives min_offset tmp : ", min_offset, " ", tmp);
        #endif

        if(tmp<0)
            throw std::runtime_error("Error: bad value in mult selection, should never happen. Please slap the developper.");

        if(j>=(int)N)
        {
            j = tmp;
            index = Selection_::selected_sorted[Selection_::thread_id][j];
        }
        marked[Selection_::thread_id][j] = true;
        max_cumulated -= qualities[index];
        if(j==min_offset+1)
        {
            min_offset = j;
            while(min_offset+1<(int)N && marked[Selection_::thread_id][min_offset+1])
                min_offset++;
        }

        #ifdef QUALITY_SELECTION_DEBUG
            logger->write("Au final : j min_offset max_cumulated index : ", j, " ", min_offset," ", max_cumulated, " ", index);
            logger->endLine();
        #endif
        Selection_::selected_sorted_reversed[Selection_::thread_id][index] = i;
        indexes[i-begin_at] =  index;
    }

    for(int i : indexes)
        Selection_::selected_sorted[Selection_::thread_id][Selection_::selected_sorted_reversed[Selection_::thread_id][i]] = i;

    #ifdef QUALITY_SELECTION_DEBUG
        logger->write("AT THE END (quality) :");
        logger->write(Vector_To_String<const std::array<int, N>&>(Selection_::selected_sorted[Selection_::thread_id]));
        logger->write(Vector_To_String<const std::array<int, N>&>(Selection_::selected_sorted_reversed[Selection_::thread_id]));
        logger->endLine();
    #endif

    return Selection_::selected_sorted[Selection_::thread_id];
}


#endif
