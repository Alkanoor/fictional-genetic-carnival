

template <size_t N, typename T>
class Selection
{
    public:
        virtual void apply(const std::array<N,T>& qualities, std::array<N,int>& selected_sorted);
};
