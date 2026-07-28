struct Vector_EraseAndMove_Overlay
{
    std::byte _pad0[4];
    void* tail;
};

static_assert(offsetof(Vector_EraseAndMove_Overlay, tail) == 0x4);

void* __thiscall Vector_EraseAndMove(Vector_EraseAndMove_Overlay* self, void* dst, void* src)
{
    if (self->tail == src)
    {
        self->tail = dst;
        return dst;
    }

    const std::size_t size = static_cast<std::size_t>(
        static_cast<char*>(self->tail) - static_cast<char*>(src));

    void* const moved = std::memmove(dst, src, size);
    self->tail = static_cast<void*>(static_cast<char*>(moved) + size);
    return dst;
}