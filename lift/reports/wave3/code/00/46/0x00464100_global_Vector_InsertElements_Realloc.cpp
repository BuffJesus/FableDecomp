#include <algorithm>
#include <cstddef>
#include <cstdint>

struct VectorInsertElementsReallocOverlay {
    std::byte pad_00[0x04];
    std::uint32_t* finish;          // +0x04
    std::uint32_t* end_of_storage;  // +0x08
};

static_assert(offsetof(VectorInsertElementsReallocOverlay, finish) == 0x04);
static_assert(offsetof(VectorInsertElementsReallocOverlay, end_of_storage) == 0x08);

extern void __thiscall Vector_Reallocate(
    VectorInsertElementsReallocOverlay* self,
    std::uint32_t* insert_at,
    std::uint32_t* value_ptr,
    std::uint8_t* masked_count_end,
    std::uint32_t requested_count,
    int zero_flag);

void __thiscall Vector_InsertElements_Realloc(
    VectorInsertElementsReallocOverlay* self,
    std::uint32_t* insert_at,
    std::uint32_t count,
    std::uint32_t* value_ptr)
{
    const std::uint32_t original_count = count;
    if (count == 0) {
        return;
    }

    if (static_cast<std::uint32_t>(
            (reinterpret_cast<std::uintptr_t>(self->end_of_storage) -
             reinterpret_cast<std::uintptr_t>(self->finish)) >> 2) < count) {
        std::uint32_t masked_count = count & 0x00FFFFFFu;
        Vector_Reallocate(
            self,
            insert_at,
            value_ptr,
            reinterpret_cast<std::uint8_t*>(&masked_count) + 3,
            original_count,
            0);
        return;
    }

    const std::uint32_t fill_value = *value_ptr;
    std::uint32_t* const old_finish = self->finish;
    const std::uint32_t trailing_count = static_cast<std::uint32_t>(
        (reinterpret_cast<std::uintptr_t>(self->finish) -
         reinterpret_cast<std::uintptr_t>(insert_at)) >> 2);

    if (count < trailing_count) {
        std::uint32_t* const src_begin = old_finish - count;
        std::uint32_t* dst = old_finish;

        for (std::uint32_t* src = src_begin; src != old_finish; ++src) {
            if (dst != nullptr) {
                *dst = *src;
            }
            ++dst;
        }

        self->finish = self->finish + count;

        int move_count = static_cast<int>(
            (reinterpret_cast<std::uintptr_t>(src_begin) -
             reinterpret_cast<std::uintptr_t>(insert_at)) >> 2);
        if (move_count > 0) {
            const std::intptr_t delta =
                reinterpret_cast<std::intptr_t>(src_begin) -
                reinterpret_cast<std::intptr_t>(old_finish);
            std::uint32_t* move_dst = old_finish;

            do {
                --move_dst;
                --move_count;
                *move_dst = *reinterpret_cast<std::uint32_t*>(
                    delta + reinterpret_cast<std::intptr_t>(move_dst));
            } while (move_count != 0);
        }

        std::uint32_t* const fill_end = insert_at + count;
        for (; insert_at != fill_end; ++insert_at) {
            *insert_at = fill_value;
        }
    } else {
        const std::uint32_t appended_count = count - trailing_count;
        count = trailing_count;

        std::fill_n(old_finish, appended_count, fill_value);
        self->finish = self->finish + appended_count;

        std::uint32_t* dst = self->finish;
        for (std::uint32_t* src = insert_at; src != old_finish; ++src) {
            if (dst != nullptr) {
                *dst = *src;
            }
            ++dst;
        }

        self->finish = self->finish + count;

        for (; insert_at != old_finish; ++insert_at) {
            *insert_at = fill_value;
        }
    }
}