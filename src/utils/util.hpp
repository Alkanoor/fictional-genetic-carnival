#include <ostream>
#include <vector>


namespace Utils
{
    template <typename T>
    std::ostream& write_vec(const std::vector<T>& to_write, std::ostream& out, char sep = ' ', bool endline = true);
}


template <typename T>
std::ostream& operator << (const std::vector<T>& to_write, std::ostream& out)
{
    return Utils::write_vec(to_write, out);
}
