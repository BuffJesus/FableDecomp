#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>

struct CRGBColour {
    std::uint32_t value;
};
static_assert(sizeof(CRGBColour) == 0x4);

struct VectorOverlay {
    CRGBColour* begin;
    CRGBColour* end;
    CRGBColour* capacity_end;
};
static_assert(offsetof(VectorOverlay, end) == 0x4);
static_assert(offsetof(VectorOverlay, capacity_end) == 0x8);
static_assert(sizeof(VectorOverlay) == 0xC);

#pragma pack(push, 1)
struct ReallocateInsertSpill {
    std::uint8_t zero;
    CRGBColour value;
};
#pragma pack(pop)

static_assert(offsetof(ReallocateInsertSpill, zero) == 0x0);
static_assert(offsetof(ReallocateInsertSpill, value) == 0x1);
static_assert(sizeof(ReallocateInsertSpill) == 0x5);

extern void __thiscall Vector_Reallocate(
    VectorOverlay* self,
    CRGBColour* position,
    const CRGBColour* value_ptr,
    const ReallocateInsertSpill* spill,
    std::uint32_t count,
    int zero_flag);

void __thiscall Vector_Insert(
    VectorOverlay* self,
    CRGBColour* position,
    std::uint32_t count,
    const CRGBColour* value_ptr) {
    if (count == 0) {
        return;
    }

    if (static_cast<std::uint32_t>(self->capacity_end - self->end) < count) {
        ReallocateInsertSpill spill{};
        spill.zero = 0;
        spill.value = *value_ptr;

        Vector_Reallocate(
            self,
            position,
            value_ptr,
            &spill,
            count,
            0);
        return;
    }

    CRGBColour* old_end = self->end;
    const std::uint32_t value_bits = value_ptr->value;
    const std::uint32_t trailing_count = static_cast<std::uint32_t>(old_end - position);

    if (count < trailing_count) {
        const std::int32_t insert_bytes = static_cast<std::int32_t>(count * 4);
        CRGBColour* move_src = old_end - count;
        CRGBColour* construct_dst = old_end;

        for (CRGBColour* src = move_src; src != old_end; ++src, ++construct_dst) {
            std::construct_at(construct_dst, *src);
        }

        self->end = self->end + count;

        std::ptrdiff_t elements_to_shift = move_src - position;
        if (elements_to_shift > 0) {
            const std::ptrdiff_t byte_delta =
                reinterpret_cast<const std::byte*>(move_src) -
                reinterpret_cast<const std::byte*>(old_end);

            do {
                --old_end;
                --elements_to_shift;
                *reinterpret_cast<std::uint32_t*>(old_end) =
                    *reinterpret_cast<const std::uint32_t*>(
                        reinterpret_cast<const std::byte*>(old_end) + byte_delta);
            } while (elements_to_shift != 0);
        }

        CRGBColour* fill_end = reinterpret_cast<CRGBColour*>(
            reinterpret_cast<std::byte*>(position) + insert_bytes);
        for (; position != fill_end; ++position) {
            *reinterpret_cast<std::uint32_t*>(position) = value_bits;
        }
    } else {
        const std::uint32_t extra_count = count - trailing_count;

        std::fill_n(
            reinterpret_cast<std::uint32_t*>(old_end),
            extra_count,
            value_bits);

        self->end = self->end + extra_count;

        CRGBColour* construct_dst = self->end;
        for (CRGBColour* src = position; src != old_end; ++src, ++construct_dst) {
            std::construct_at(construct_dst, *src);
        }

        self->end = self->end + trailing_count;

        for (; position != old_end; ++position) {
            *reinterpret_cast<std::uint32_t*>(position) = value_bits;
        }
    }
}