#include <cstdlib>

void __cdecl FreePointerArray(void** begin, void** end)
{
    for (; begin < end; ++begin) {
        if (*begin != nullptr) {
            std::free(*begin);
        }
    }
}