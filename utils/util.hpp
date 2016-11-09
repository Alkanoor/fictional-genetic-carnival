#ifndef UTIL_HPP
#define UTIL_HPP


#include <functional>
#include <algorithm>
#include <ostream>
#include <vector>
#include <array>


namespace Utils
{
    template <size_t N, typename U>
    void apply(int begin_at, std::array<int, N>& sorted, std::array<U, N>& sorted_reversed, const std::function<U(int, int)>& functor_to_be_applied_on_ranks);

    template <size_t N, typename T, typename U>
    void index_after_sorting(const std::array<T, N>& qualities, int begin_at, std::array<int, N>& sorted, std::array<U, N>& sorted_reversed, const std::function<U(int, int)>& functor_to_be_applied_on_ranks);

    template <size_t N, typename T>
    void index_after_sorting(const std::array<T, N>& qualities, int begin_at, std::array<int, N>& sorted, std::array<int, N>& sorted_reversed);

    template <typename T>
    std::ostream& write_vec(const std::vector<T>& to_write, std::ostream& out, char sep = ' ', bool endline = false);

    template <typename T, size_t N>
    std::ostream& write_vec(const std::array<T,N>& to_write, std::ostream& out, char sep = ' ', bool endline = false);
}

template <size_t N, typename U>
void Utils::apply(int begin_at, std::array<int, N>& sorted, std::array<U, N>& sorted_reversed, const std::function<U(int, int)>& functor_to_be_applied_on_ranks)
{
    for(int i=begin_at; i<N; i++)
        sorted_reversed[sorted[i]] = functor_to_be_applied_on_ranks(i, N-1);
}

template <size_t N, typename T, typename U>
void Utils::index_after_sorting(const std::array<T, N>& qualities, int begin_at, std::array<int, N>& sorted, std::array<U, N>& sorted_reversed, const std::function<U(int, int)>& functor_to_be_applied_on_ranks)
{
    auto begin = sorted.begin();
    begin += begin_at;
    std::iota(begin, sorted.end(), 0);
    std::sort(begin, sorted.end(), [&](int a, int b) {return qualities[a+begin_at] > qualities[b+begin_at];});
    apply(begin_at, sorted, sorted_reversed, functor_to_be_applied_on_ranks);
}

template <size_t N, typename T>
void Utils::index_after_sorting(const std::array<T, N>& qualities, int begin_at, std::array<int, N>& sorted, std::array<int, N>& sorted_reversed)
{
    auto begin = sorted.begin();
    begin += begin_at;
    std::iota(begin, sorted.end(), 0);
    std::sort(begin, sorted.end(), [&](int a, int b) {return qualities[a+begin_at] > qualities[b+begin_at];});
    for(int i=begin_at; i<N; i++)
        sorted_reversed[sorted[i]] = i;
}

template <typename T>
std::ostream& Utils::write_vec(const std::vector<T>& to_write, std::ostream& out, char sep, bool endline)
{
    for(const T& it : to_write)
    {
        out<<it<<sep;
        if(endline)
            out<<std::endl;
    }
    return out;
}

template <typename T, size_t N>
std::ostream& Utils::write_vec(const std::array<T,N>& to_write, std::ostream& out, char sep, bool endline)
{
    for(const T& it : to_write)
    {
        out<<it<<sep;
        if(endline)
            out<<std::endl;
    }
    return out;
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& to_write)
{
    return Utils::write_vec(to_write, out);
}

template <typename T, size_t N>
std::ostream& operator << (std::ostream& out, const std::array<T,N>& to_write)
{
    return Utils::write_vec(to_write, out);
}

#endif
