#include <cstdlib>

void __cdecl Vector_FreeElementPointers(void** first, void** last)
{
    for (; first < last; ++first)
    {
        if (*first != nullptr)
        {
            std::free(*first);
        }
    }
}