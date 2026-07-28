#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

struct VectorOverlay {
    std::uint32_t* begin;
    std::uint32_t* end;
    std::uint32_t* capacity_end;
};

static_assert(offsetof(VectorOverlay, begin) == 0x0);
static_assert(offsetof(VectorOverlay, end) == 0x4);
static_assert(offsetof(VectorOverlay, capacity_end) == 0x8);
static_assert(sizeof(VectorOverlay) == 0xC);

void __thiscall Vector_InsertN(
    VectorOverlay* self,
    void* insert_at,
    const std::uint32_t* value,
    std::uint32_t /*unused*/,
    std::uint32_t count,
    char skip_suffix_copy)
{
    const std::uint32_t current_count =
        (static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self->end) -
                                    reinterpret_cast<std::uintptr_t>(self->begin)) >>
         2);

    std::uint32_t new_capacity = current_count;
    if (current_count < count) {
        new_capacity = count;
    }
    new_capacity = new_capacity + current_count;

    std::uint32_t* new_begin;
    if (new_capacity == 0) {
        new_begin = nullptr;
    } else {
        new_begin = static_cast<std::uint32_t*>(std::malloc(new_capacity * 4));
    }

    void* const old_begin = self->begin;
    std::uint32_t* dst = new_begin;

    if (insert_at != old_begin) {
        void* const moved_prefix = std::memmove(
            new_begin,
            old_begin,
            static_cast<std::size_t>(
                reinterpret_cast<std::uintptr_t>(insert_at) -
                reinterpret_cast<std::uintptr_t>(old_begin)));
        dst = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(moved_prefix) +
            (reinterpret_cast<std::uintptr_t>(insert_at) -
             reinterpret_cast<std::uintptr_t>(old_begin)));
    }

    for (std::uint32_t remaining = count; remaining != 0; --remaining) {
        *dst = *value;
        ++dst;
    }

    if ((skip_suffix_copy == '\0') && (self->end != insert_at)) {
        const std::size_t suffix_size =
            static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(self->end) -
                                     reinterpret_cast<std::uintptr_t>(insert_at));
        void* const moved_suffix = std::memmove(dst, insert_at, suffix_size);
        dst = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(moved_suffix) + suffix_size);
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->end = dst;
    self->begin = new_begin;
    self->capacity_end = new_begin + new_capacity;
}