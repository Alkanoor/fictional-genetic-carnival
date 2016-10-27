#include "selection.hpp"


template <size_t N, typename T>
class Rank_Selection : public Selection<N,T>
{
    public:
        void apply(const std::array<N,T>& qualities, std::array<N,int>& selected_sorted);
};
