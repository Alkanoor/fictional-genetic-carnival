#include "rank_selection.hpp"


template <size_t N, typename T>
class Elit_Selection : public Selection<N,T>
{
    public:
        Elit_Selection(int nb_to_keep=1, std::shared_ptr<Selection<N,T> > base=std::shared_ptr<Selection<N,T> >(new Rank_Selection<N,T>()));

        void apply(const std::array<N,T>& qualities, std::array<N,int>& selected_sorted);

    private:
        int number_individuals_to_keep;
        std::shared_ptr<Selection<N,T> > default_selection;
};
