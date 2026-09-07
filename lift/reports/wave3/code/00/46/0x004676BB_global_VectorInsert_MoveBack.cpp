#include <cstddef>
#include <cstdint>
#include <cstring>

struct VectorInsert_MoveBack_Overlay {
    std::byte pad_0[0x4];
    void* end;
};

static_assert(sizeof(void*) == 4);
static_assert(offsetof(VectorInsert_MoveBack_Overlay, end) == 0x4);

void* __thiscall VectorInsert_MoveBack(void* self, void* destination, void* source)
{
    auto* const vec = static_cast<VectorInsert_MoveBack_Overlay*>(self);
    void* newEnd = destination;

    if (vec->end != source) {
        const std::size_t size =
            static_cast<std::size_t>(
                reinterpret_cast<std::uintptr_t>(vec->end) -
                reinterpret_cast<std::uintptr_t>(source));

        newEnd = std::memmove(destination, source, size);
        newEnd = static_cast<void*>(
            reinterpret_cast<std::uintptr_t>(newEnd) + size);
    }

    vec->end = newEnd;
    return destination;
}