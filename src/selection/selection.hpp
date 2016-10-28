

template <size_t N, typename T, size_t N_threads = 1>
class Selection
{
    public:
        Selection(int thread_id=0);

        virtual const std::array<N,int>& apply(const std::array<N,T>& qualities, int begin_at=0) = 0;

        const std::array<N,int>& get_sorted()
        {return selected_sorted;}

        const std::array<N,int>& get_sorted_reversed()
        {return selected_sorted_reversed;}

    protected:
        int thread_id;

        static std::array<std::array<N,int>, N_threads> selected_sorted;
        static std::array<std::array<N,int>, N_threads> selected_sorted_reversed;
};


template <size_t N, typename T, size_t N_threads>
Selection<N,T,N_threads>::Selection(int id) :
    thread_id(id)
{
    assert(thread_id<N_threads);
}
