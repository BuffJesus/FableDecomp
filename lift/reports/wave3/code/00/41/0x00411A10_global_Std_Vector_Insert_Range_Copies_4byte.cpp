#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct StdVector4ByteOverlay {
    std::uint32_t* begin;
    std::uint32_t* end;
    std::uint32_t* capacity_end;
};

static_assert(offsetof(StdVector4ByteOverlay, begin) == 0x0);
static_assert(offsetof(StdVector4ByteOverlay, end) == 0x4);
static_assert(offsetof(StdVector4ByteOverlay, capacity_end) == 0x8);
static_assert(sizeof(StdVector4ByteOverlay) == 0xC);

void __thiscall Std_Vector_Insert_Range_Copies_4byte(
    StdVector4ByteOverlay* self,
    std::uint32_t* insert_at,
    const std::uint32_t* copy_value,
    std::uint32_t /*unused_param_4*/,
    std::uint32_t copy_count,
    char skip_suffix_copy)
{
    std::uint32_t remaining_copies = copy_count;
    std::uint32_t old_size =
        static_cast<std::uint32_t>((reinterpret_cast<std::uintptr_t>(self->end) -
                                    reinterpret_cast<std::uintptr_t>(self->begin)) >> 2);

    std::uint32_t growth = copy_count;
    if (copy_count <= old_size) {
        growth = old_size;
    }

    std::uint32_t new_capacity = growth + old_size;

    std::uint32_t* new_begin;
    if (new_capacity == 0) {
        new_begin = nullptr;
    } else {
        new_begin = static_cast<std::uint32_t*>(std::malloc(new_capacity * 4));
    }

    std::uint32_t* write = new_begin;
    std::uint32_t* read = self->begin;

    for (; read != insert_at; ++read) {
        if (write != nullptr) {
            *write = *read;
        }
        ++write;
    }

    if (remaining_copies == 1) {
        if (write != nullptr) {
            *write = *copy_value;
        }
        ++write;
    } else {
        for (; remaining_copies != 0; --remaining_copies) {
            if (write != nullptr) {
                *write = *copy_value;
            }
            ++write;
        }
    }

    if ((skip_suffix_copy == '\0') && (insert_at != self->end)) {
        const auto delta =
            reinterpret_cast<std::intptr_t>(insert_at) - reinterpret_cast<std::intptr_t>(write);

        do {
            if (write != nullptr) {
                *write = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::intptr_t>(write) + delta);
            }
            ++write;
        } while (reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::intptr_t>(write) + delta) != self->end);
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->end = write;
    self->begin = new_begin;
    self->capacity_end = new_begin + new_capacity;
}