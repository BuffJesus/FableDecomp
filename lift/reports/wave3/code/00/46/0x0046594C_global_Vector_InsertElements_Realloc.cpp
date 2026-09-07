#include <algorithm>
#include <cstddef>
#include <cstdint>

struct VectorInsertElementsReallocOverlay {
    std::byte pad_00[0x04];
    unsigned long* finish;
    unsigned long* end_of_storage;
};

static_assert(offsetof(VectorInsertElementsReallocOverlay, finish) == 0x04);
static_assert(offsetof(VectorInsertElementsReallocOverlay, end_of_storage) == 0x08);

extern void Vector_Reallocate(
    unsigned long* position,
    unsigned long* value_ptr,
    void* packed_count_end,
    unsigned int count,
    int zero
);

void __thiscall Vector_InsertElements_Realloc(
    void* self_,
    unsigned long* position,
    unsigned int count,
    unsigned long* value_ptr
) {
    auto* const self = static_cast<VectorInsertElementsReallocOverlay*>(self_);
    unsigned int requested_count = count;

    if (count != 0) {
        if (static_cast<unsigned int>(self->end_of_storage - self->finish) < count) {
            std::uint32_t masked_count = count & 0x00FFFFFFu;
            Vector_Reallocate(
                position,
                value_ptr,
                reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(&masked_count) + 3),
                requested_count,
                0
            );
        } else {
            const unsigned long fill_value = *value_ptr;
            unsigned long* const old_finish = self->finish;
            unsigned int elems_after = static_cast<unsigned int>(self->finish - position);

            if (count < elems_after) {
                unsigned long* const move_src_begin = old_finish - count;
                unsigned long* move_dst = old_finish;

                for (unsigned long* move_src = move_src_begin; move_src != old_finish; ++move_src) {
                    if (move_dst != nullptr) {
                        *move_dst = *move_src;
                    }
                    ++move_dst;
                }

                self->finish = self->finish + count;

                int prefix_count = static_cast<int>(move_src_begin - position);
                if (0 < prefix_count) {
                    const std::intptr_t delta =
                        reinterpret_cast<std::intptr_t>(move_src_begin) -
                        reinterpret_cast<std::intptr_t>(old_finish);

                    unsigned long* tail = old_finish;
                    do {
                        --tail;
                        --prefix_count;
                        *tail = *reinterpret_cast<unsigned long*>(reinterpret_cast<std::intptr_t>(tail) + delta);
                    } while (prefix_count != 0);
                }

                for (unsigned long* it = position, *end = position + count; it != end; ++it) {
                    *it = fill_value;
                }
            } else {
                const unsigned int extra = count - elems_after;
                count = elems_after;

                std::fill_n(old_finish, extra, fill_value);
                self->finish = self->finish + extra;

                unsigned long* copy_dst = self->finish;
                for (unsigned long* src = position; src != old_finish; ++src) {
                    if (copy_dst != nullptr) {
                        *copy_dst = *src;
                    }
                    ++copy_dst;
                }

                self->finish = self->finish + count;

                for (unsigned long* it = position; it != old_finish; ++it) {
                    *it = fill_value;
                }
            }
        }
    }
}