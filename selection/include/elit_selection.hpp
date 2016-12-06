#ifndef ELIT_SELECTION_HPP
#define ELIT_SELECTION_HPP


//#define ELIT_SELECTION_DEBUG

#include <cassert>
#include <memory>

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

        static std::array<std::array<T,N>, N_threads> temp_sorted_qualities;

        typedef Selection<N,T,N_threads> Selection_;
};


template <size_t N, typename T, size_t N_threads>
std::array<std::array<T,N>, N_threads> Elit_Selection<N,T,N_threads>::temp_sorted_qualities;


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
    assert(number_individuals_to_keep<=(int)N);
}

template <size_t N, typename T, size_t N_threads>
const std::array<int,N>& Elit_Selection<N,T,N_threads>::apply(const std::array<T,N>& qualities, int begin_at, bool already_sorted) throw ()
{
    if(!already_sorted)
        Utils::index_after_sorting(qualities, begin_at, Selection_::selected_sorted[Selection_::thread_id], Selection_::selected_sorted_reversed[Selection_::thread_id]);

    #ifdef ELIT_SELECTION_DEBUG
        auto logger = Easy_Log_In_File_Debug::getInfoLog();
        logger->write("AT THE Beginning (elit) : ", number_individuals_to_keep);
        logger->write(Vector_To_String<const std::array<int, N> >(&Selection_::selected_sorted[Selection_::thread_id]));
        logger->endLine();
    #endif

    default_selection->set_selected_sorted(Selection_::selected_sorted[Selection_::thread_id]);
    const std::array<int,N>& temp = default_selection->apply(qualities, number_individuals_to_keep, true);

    #ifdef ELIT_SELECTION_DEBUG
        logger->write("AT THE Middle (elit) :");
        logger->write(Vector_To_String<const std::array<int, N> >(&temp));
        logger->endLine();
    #endif

    for(int i=0; i<number_individuals_to_keep; i++)
        Selection_::selected_sorted_reversed[Selection_::thread_id][Selection_::selected_sorted[Selection_::thread_id][i]] = i;

    for(int i=number_individuals_to_keep; i<(int)N; i++)
    {
        Selection_::selected_sorted[Selection_::thread_id][i] = temp[i];
        Selection_::selected_sorted_reversed[Selection_::thread_id][temp[i]] = i;
    }

    return Selection_::selected_sorted[Selection_::thread_id];
}


#endif
