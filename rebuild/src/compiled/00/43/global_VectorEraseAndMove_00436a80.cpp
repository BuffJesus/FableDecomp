#include <string.h>

struct Vector_EraseAndMove_Overlay
{
    unsigned char unused[4];
    void* tail;
};

void* __fastcall Vector_EraseAndMove(Vector_EraseAndMove_Overlay* self, void*, void* dst, void* src)
{
    register int size = (int)self->tail;
    if ((void*)size == src) {
        self->tail = dst;
        return dst;
    }
    size -= (int)src;
    void* moved = memmove(dst, src, size);
    self->tail = (char*)moved + size;
    return dst;
}