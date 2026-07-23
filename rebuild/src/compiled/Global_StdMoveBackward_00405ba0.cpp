#include <stddef.h>
#include <string.h>

void* __fastcall Std_Move_Backward(void* first, void* last, void* dest)
{
    if (last != first)
    {
        size_t count = reinterpret_cast<size_t>(last);
        count -= reinterpret_cast<size_t>(first);
        dest = memmove(dest, first, count);
        dest = static_cast<unsigned char*>(dest) + count;
    }
    return dest;
}
