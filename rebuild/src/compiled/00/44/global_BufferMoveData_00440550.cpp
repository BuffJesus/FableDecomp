#include <string.h>

struct BufferOverlay
{
    unsigned char unused[4];
    void* tail;
};

void* __fastcall Buffer_MoveData(BufferOverlay* self, void*, void* dst, void* src)
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