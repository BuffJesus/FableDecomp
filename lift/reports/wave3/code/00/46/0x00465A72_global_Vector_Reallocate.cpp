#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct VectorOverlay {
    std::uint32_t* begin;
    std::uint32_t* end;
    std::uint32_t* capacity_end;
};

static_assert(offsetof(VectorOverlay, begin) == 0x0);
static_assert(offsetof(VectorOverlay, end) == 0x4);
static_assert(offsetof(VectorOverlay, capacity_end) == 0x8);
static_assert(sizeof(VectorOverlay) == 0xC);

void __thiscall Vector_Reallocate(
    VectorOverlay* self,
    std::uint32_t* insert_pos,
    const std::uint32_t* value,
    std::uint32_t /*unused*/,
    std::uint32_t insert_count,
    char skip_tail_copy)
{
    const std::uint32_t old_count =
        static_cast<std::uint32_t>((reinterpret_cast<std::uintptr_t>(self->end) -
                                    reinterpret_cast<std::uintptr_t>(self->begin)) >>
                                   2);

    const std::uint32_t growth = (insert_count <= old_count) ? old_count : insert_count;
    const std::uint32_t new_capacity = growth + old_count;

    std::uint32_t* new_begin;
    if (new_capacity == 0) {
        new_begin = nullptr;
    } else {
        new_begin = static_cast<std::uint32_t*>(std::malloc(new_capacity * 4));
    }

    std::uint32_t* write = new_begin;
    std::uint32_t* read = self->begin;

    for (; read != insert_pos; ++read) {
        if (write != nullptr) {
            *write = *read;
        }
        ++write;
    }

    if (insert_count == 1) {
        if (write != nullptr) {
            *write = *value;
        }
        ++write;
    } else {
        for (; insert_count != 0; --insert_count) {
            if (write != nullptr) {
                *write = *value;
            }
            ++write;
        }
    }

    if (skip_tail_copy == '\0') {
        std::uint32_t* const old_end = self->end;
        if (insert_pos != old_end) {
            const std::intptr_t delta =
                reinterpret_cast<std::uintptr_t>(insert_pos) -
                reinterpret_cast<std::uintptr_t>(write);

            do {
                if (write != nullptr) {
                    *write = *reinterpret_cast<std::uint32_t*>(
                        reinterpret_cast<std::uintptr_t>(write) + delta);
                }
                ++write;
            } while (reinterpret_cast<std::uint32_t*>(
                         reinterpret_cast<std::uintptr_t>(write) + delta) != old_end);
        }
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->begin = new_begin;
    self->end = write;
    self->capacity_end = new_begin + new_capacity;
}