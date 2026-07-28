#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct Vector16Storage {
    struct Element {
        std::uint32_t u0;
        std::uint32_t u1;
        std::uint32_t u2;
        std::uint32_t u3;
    };

    Element* begin;
    Element* end;
    Element* capacity_end;
};

static_assert(sizeof(Vector16Storage::Element) == 0x10);
static_assert(offsetof(Vector16Storage, begin) == 0x00);
static_assert(offsetof(Vector16Storage, end) == 0x04);
static_assert(offsetof(Vector16Storage, capacity_end) == 0x08);

void __thiscall Std_Vector_Insert_Range_16Bytes(
    Vector16Storage* self,
    Vector16Storage::Element* insert_at,
    const Vector16Storage::Element* value,
    std::uint32_t /*unused*/,
    std::size_t count,
    char skip_tail_copy)
{
    const std::size_t old_size =
        (static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(self->end)) -
         static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(self->begin))) >> 4;

    const std::size_t new_capacity = old_size + ((count <= old_size) ? old_size : count);

    auto* new_begin = (new_capacity == 0)
        ? nullptr
        : static_cast<Vector16Storage::Element*>(std::malloc(new_capacity * 0x10));

    auto* write = std::copy(self->begin, insert_at, new_begin);

    if (count == 1) {
        if (write != nullptr) {
            write->u0 = value->u0;
            write->u1 = value->u1;
            write->u2 = value->u2;
            write->u3 = value->u3;
        }
        write += 1;
    } else {
        write = std::fill_n(write, count, *value);
    }

    if (skip_tail_copy == '\0') {
        write = std::copy(insert_at, self->end, write);
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->begin = new_begin;
    self->end = write;
    self->capacity_end = new_begin + new_capacity;
}