#include <stddef.h>
#include <string.h>

void* __fastcall Std_Move_Backward(void* first, void* last, void* dest)
{
    if (last != first)
    {
        size_t count = static_cast<unsigned char*>(last) - static_cast<unsigned char*>(first);
        dest = memmove(dest, first, count);
        dest = static_cast<unsigned char*>(dest) + count;
    }
    return dest;
}
