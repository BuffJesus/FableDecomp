#include <stddef.h>
#include <string.h>
void* __fastcall Std_Move_Backward(void* first, void* last, void* dest)
{
    if (last != first)
    {
        size_t count = (unsigned char*)last - (unsigned char*)first;
        dest = memmove(dest, first, count);
        dest = (unsigned char*)dest + count;
    }
    return dest;
}
