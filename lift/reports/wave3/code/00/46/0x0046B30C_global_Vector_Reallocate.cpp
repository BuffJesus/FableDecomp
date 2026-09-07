#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct Pair8 {
    std::uint32_t lo;
    std::uint32_t hi;
};
static_assert(sizeof(Pair8) == 0x8);

struct VectorOverlay {
    Pair8* begin;
    Pair8* end;
    Pair8* capacity_end;
};
static_assert(offsetof(VectorOverlay, begin) == 0x0);
static_assert(offsetof(VectorOverlay, end) == 0x4);
static_assert(offsetof(VectorOverlay, capacity_end) == 0x8);
static_assert(sizeof(VectorOverlay) == 0xC);

void __thiscall Vector_Reallocate(
    VectorOverlay* self,
    Pair8* insert_pos,
    const Pair8* value,
    std::uint32_t /*unused*/,
    std::uint32_t insert_count,
    char copy_suffix)
{
    const std::uint32_t current_count = static_cast<std::uint32_t>(
        (reinterpret_cast<std::uintptr_t>(self->end) - reinterpret_cast<std::uintptr_t>(self->begin)) >> 3);

    const std::uint32_t growth = (insert_count <= current_count) ? current_count : insert_count;
    const std::uint32_t new_capacity = growth + current_count;

    Pair8* new_begin;
    if (new_capacity == 0) {
        new_begin = nullptr;
    } else {
        new_begin = static_cast<Pair8*>(std::malloc(static_cast<std::size_t>(new_capacity) * sizeof(Pair8)));
    }

    std::uintptr_t write_cursor = reinterpret_cast<std::uintptr_t>(new_begin);

    Pair8* scan = self->begin;
    while (scan != insert_pos) {
        if (write_cursor != 0) {
            auto* const dst = reinterpret_cast<Pair8*>(write_cursor);
            dst->lo = scan->lo;
            dst->hi = scan->hi;
        }
        ++scan;
        write_cursor += sizeof(Pair8);
    }

    if (insert_count == 1) {
        if (write_cursor != 0) {
            auto* const dst = reinterpret_cast<Pair8*>(write_cursor);
            dst->lo = value->lo;
            dst->hi = value->hi;
        }
        write_cursor += sizeof(Pair8);
    } else {
        for (; insert_count != 0; --insert_count) {
            if (write_cursor != 0) {
                auto* const dst = reinterpret_cast<Pair8*>(write_cursor);
                dst->lo = value->lo;
                dst->hi = value->hi;
            }
            write_cursor += sizeof(Pair8);
        }
    }

    if (copy_suffix == '\0') {
        Pair8* const old_end = self->end;
        if (insert_pos != old_end) {
            const std::intptr_t delta =
                reinterpret_cast<std::intptr_t>(insert_pos) - static_cast<std::intptr_t>(write_cursor);

            do {
                if (write_cursor != 0) {
                    auto* const dst = reinterpret_cast<Pair8*>(write_cursor);
                    const auto* const src =
                        reinterpret_cast<const Pair8*>(delta + static_cast<std::intptr_t>(write_cursor));
                    dst->lo = src->lo;
                    dst->hi = src->hi;
                }
                write_cursor += sizeof(Pair8);
            } while (reinterpret_cast<const Pair8*>(delta + static_cast<std::intptr_t>(write_cursor)) != old_end);
        }
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }

    self->begin = new_begin;
    self->end = reinterpret_cast<Pair8*>(write_cursor);
    self->capacity_end = reinterpret_cast<Pair8*>(
        reinterpret_cast<std::uintptr_t>(new_begin) +
        static_cast<std::uintptr_t>(new_capacity) * sizeof(Pair8));
}