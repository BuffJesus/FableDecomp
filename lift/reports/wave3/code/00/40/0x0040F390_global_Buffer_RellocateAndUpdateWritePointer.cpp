#include <cstddef>
#include <cstdint>
#include <cstring>

struct BufferWritePointerOverlay
{
    std::byte pad_00[4];
    void* writePointer;
};

static_assert(offsetof(BufferWritePointerOverlay, writePointer) == 0x4);

void* __thiscall Buffer_RellocateAndUpdateWritePointer(
    BufferWritePointerOverlay* self,
    void* param_2,
    void* param_3)
{
    if (self->writePointer == param_3)
    {
        self->writePointer = param_2;
        return param_2;
    }

    const std::size_t size =
        static_cast<std::size_t>(
            static_cast<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(self->writePointer)) -
            static_cast<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(param_3)));

    void* const moved = std::memmove(param_2, param_3, size);
    self->writePointer = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(moved)) + size);

    return param_2;
}