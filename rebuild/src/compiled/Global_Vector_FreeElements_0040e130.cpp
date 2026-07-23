#include <stdlib.h>

void __stdcall Vector_FreeElements(void** begin, void** end)
{
    for (; begin < end; ++begin)
    {
        if (*begin != 0)
            free(*begin);
    }
}
