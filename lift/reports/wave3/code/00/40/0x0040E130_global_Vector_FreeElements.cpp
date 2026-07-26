#include <cstdlib>

void __stdcall Vector_FreeElements(void** begin, void** end)
{
    for (; begin < end; ++begin) {
        if (*begin != nullptr) {
            std::free(*begin);
        }
    }
}