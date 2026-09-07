#include <string.h>
#include <stdio.h>

struct VectorState { unsigned char unused[4]; void* tail; };

static void* EraseAndMove(VectorState* self, void* dst, void* src)
{
    if (self->tail == src) {
        self->tail = dst;
        return dst;
    }
    int size = (char*)self->tail - (char*)src;
    void* moved = memmove(dst, src, size);
    self->tail = (char*)moved + size;
    return dst;
}

int main()
{
    char data[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    VectorState state;
    state.tail = data + 10;
    if (EraseAndMove(&state, data + 2, data + 5) != data + 2) return 1;
    if (state.tail != data + 7 || data[2] != 6 || data[6] != 10) return 2;
    state.tail = data + 4;
    if (EraseAndMove(&state, data + 1, data + 4) != data + 1) return 3;
    if (state.tail != data + 1) return 4;
    puts("BUFFER_RELOCATE_WRITE_PASS");
    return 0;
}