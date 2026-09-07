#include <string.h>

struct BufferRelocateOverlay
{
    unsigned char unused[4];
    void* end;
};

void* __fastcall Buffer_Relocate(BufferRelocateOverlay* self, void*, void* dst, void* src)
{
    register int size = (int)self->end;
    if ((void*)size == src) {
        self->end = dst;
        return dst;
    }
    size -= (int)src;
    void* moved = memmove(dst, src, size);
    self->end = (char*)moved + size;
    return dst;
}