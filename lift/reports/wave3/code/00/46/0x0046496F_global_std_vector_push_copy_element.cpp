struct StdVectorPushCopyElementOverlay {
    std::byte _pad0[4];
    void* end;
};

static_assert(offsetof(StdVectorPushCopyElementOverlay, end) == 0x4);

void* __thiscall std_vector_push_copy_element(
    StdVectorPushCopyElementOverlay* self,
    void* param_2,
    const void* param_3)
{
    void* result = param_2;

    if (self->end != param_3) {
        const std::size_t size =
            static_cast<std::size_t>(
                reinterpret_cast<std::uintptr_t>(self->end) -
                reinterpret_cast<std::uintptr_t>(param_3));

        result = std::memmove(param_2, param_3, size);
        result = static_cast<void*>(static_cast<std::byte*>(result) + size);
    }

    self->end = result;
    return param_2;
}