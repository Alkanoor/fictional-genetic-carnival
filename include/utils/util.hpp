#include <functional>
#include <ostream>
#include <vector>
#include <array>


namespace Utils
{
    template <size_t N, typename T, typename U>
    void index_after_sorting(const std::vector<T>& qualities, int begin_at, const std::array<int, N>& sorted, const std::array<U, N>& sorted_reversed, const std::function<U(int, int)>& functor_to_be_applied_on_ranks);

    template <size_t N, typename T>
    void index_after_sorting(const std::vector<T>& qualities, int begin_at, const std::array<int, N>& sorted, const std::array<int, N>& sorted_reversed);

    template <typename T>
    std::ostream& write_vec(const std::vector<T>& to_write, std::ostream& out, char sep = ' ', bool endline = true);
}


template <size_t N, typename T, typename U>
void Utils::index_after_sorting<N,T,U>(const std::vector<T>& qualities, int begin_at, const std::array<int, N>& sorted, const std::array<U, N>& sorted_reversed, const std::function<U(int, int)>& functor_to_be_applied_on_ranks);
{
    auto begin = sorted.begin();
    begin += begin_at;
    std::iota(begin, sorted.end(), 0);
    std::sort(begin, sorted.end(), [&](int a, int b) {return qualities[a+begin_at] < qualities[b+begin_at];});
    for(int i=begin_at; i<N; i++)
        sorted_reversed[sorted[i]] = functor_to_be_applied_on_ranks(i, N-1);
}

template <size_t N, typename T>
void Utils::index_after_sorting<N,T,U>(const std::vector<T>& qualities, int begin_at, const std::array<int, N>& sorted, const std::array<int, N>& sorted_reversed);
{
    auto begin = sorted.begin();
    begin += begin_at;
    std::iota(begin, sorted.end(), 0);
    std::sort(begin, sorted.end(), [&](int a, int b) {return qualities[a+begin_at] < qualities[b+begin_at];});
    for(int i=begin_at; i<N; i++)
        sorted_reversed[sorted[i]] = i;
}

template <typename T>
std::ostream& Utils::write_vec<T>(const std::vector<T>& to_write, std::ostream& out, char sep, bool endline)
{
    for(const T& it : to_write)
    {
        out<<*it<<sep;
        if(endline)
            out<<std::endl;
    }
    return out;
}

template <typename T>
std::ostream& operator << (const std::vector<T>& to_write, std::ostream& out)
{
    return Utils::write_vec(to_write, out);
}
