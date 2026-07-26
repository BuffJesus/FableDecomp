#include <stddef.h>
#include <string.h>

PERMPRAGMA(#pragma optimize("s", on)|#pragma optimize("t", on)|)

void* __fastcall Std_Move_Backward(void* first, void* last, void* dest)
{
    if (last != first)
    {
        size_t count = PERM(static_cast<unsigned char*>(last) - static_cast<unsigned char*>(first)|(size_t)last - (size_t)first|(char*)last - (char*)first|reinterpret_cast<size_t>(last) - reinterpret_cast<size_t>(first));
        dest = memmove(dest, first, count);
        dest = static_cast<unsigned char*>(dest) + count;
    }
    return dest;
}
