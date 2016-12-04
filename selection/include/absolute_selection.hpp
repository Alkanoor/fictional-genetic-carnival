#ifndef Absolute_Selection_HPP
#define Absolute_Selection_HPP


//#define Absolute_Selection_DEBUG


#include "utils/util.hpp"
#include "selection.hpp"


///*******************************************************************************************************************************************************************
/// Class that implements an absolut selection (individuals are sorted by quality)
///  N T N_threads   template parameters are the same as those from Selection class
///*******************************************************************************************************************************************************************

template <size_t N, typename T, size_t N_threads = 1>
class Absolute_Selection : public Selection<N,T,N_threads>
{
    public:
        Absolute_Selection(int thread_id=0);

        const std::array<int,N>& apply(const std::array<T,N>& qualities, int begin_at=0, bool already_sorted=false) throw ();

    private:
        typedef Selection<N,T,N_threads> Selection_;
};


template <size_t N, typename T, size_t N_threads>
Absolute_Selection<N,T,N_threads>::Absolute_Selection(int thread_id) :
    Selection<N,T,N_threads>(thread_id)
{}

template <size_t N, typename T, size_t N_threads>
const std::array<int,N>& Absolute_Selection<N,T,N_threads>::apply(const std::array<T,N>& qualities, int begin_at, bool already_sorted) throw ()
{
    if(!already_sorted)
        Utils::index_after_sorting(qualities, begin_at, Selection_::selected_sorted[Selection_::thread_id], Selection_::selected_sorted_reversed[Selection_::thread_id]);

    #ifdef Absolute_Selection_DEBUG
        auto logger = Easy_Log_In_File_Debug::getInfoLog();
        logger->write("In absolut selection : ", Vector_To_String<const std::array<int, N> >(&Selection_::selected_sorted[Selection_::thread_id]));
        logger->endLine();
    #endif

    return Selection_::selected_sorted[Selection_::thread_id];
}


#endif
