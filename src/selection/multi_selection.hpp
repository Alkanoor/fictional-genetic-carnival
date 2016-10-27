#include "selection.hpp"


template <size_t M, size_t N, typename T, typename U>
class Multi_Selection : public Selection<N,T>
{
    public:
        Multi_Selection(const std::array<M, std::shared_ptr<Selection<N,T> > >& selections, const std::array<M, U>& coeffs);

        void apply(const std::array<N,T>& qualities, std::array<N,int>& selected_sorted);

    private:
        std::array<M, std::shared_ptr<Selection<N,T> > > selections;
        std::array<M, U> selection_coeffs;
};
