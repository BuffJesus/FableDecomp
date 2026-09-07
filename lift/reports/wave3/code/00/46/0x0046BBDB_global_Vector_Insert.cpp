#include <algorithm>
#include <cstddef>
#include <cstdint>

struct Vector12Element {
    std::uint32_t word0;
    std::uint32_t word1;
    std::uint32_t word2;
};

static_assert(sizeof(Vector12Element) == 0x0C);

struct Vector12Overlay {
    std::byte unknown_0[0x04];
    Vector12Element* finish;          // +0x04
    Vector12Element* end_of_storage;  // +0x08
};

static_assert(offsetof(Vector12Overlay, finish) == 0x04);
static_assert(offsetof(Vector12Overlay, end_of_storage) == 0x08);
static_assert(sizeof(Vector12Overlay) == 0x0C);

extern void __thiscall Vector_Reallocate(
    Vector12Overlay* self,
    std::uint32_t* insert_at,
    std::uint32_t* value_ptr,
    void* count_byte_ptr_plus_3,
    std::uint32_t count,
    int zero
);

void __thiscall Vector_Insert(
    Vector12Overlay* self,
    std::uint32_t* insert_at,
    std::uint32_t count,
    std::uint32_t* value_ptr
) {
    std::uint32_t original_count = count;
    if (count == 0) {
        return;
    }

    if (static_cast<std::uint32_t>(self->end_of_storage - self->finish) < count) {
        count &= 0x00FFFFFFu;
        Vector_Reallocate(
            self,
            insert_at,
            value_ptr,
            reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(&count) + 3),
            original_count,
            0
        );
        return;
    }

    Vector12Element value{
        value_ptr[0],
        value_ptr[1],
        value_ptr[2],
    };

    auto* const position = reinterpret_cast<Vector12Element*>(insert_at);
    Vector12Element* finish = self->finish;
    std::uint32_t trailing_count = static_cast<std::uint32_t>(finish - position);

    if (count < trailing_count) {
        Vector12Element* move_src = finish - count;
        Vector12Element* move_dst = finish;

        for (Vector12Element* src = move_src; src != finish; ++src) {
            if (move_dst != nullptr) {
                *move_dst = *src;
            }
            ++move_dst;
        }

        const std::intptr_t prefix_count = move_src - position;
        self->finish = self->finish + count;

        if (0 < prefix_count) {
            Vector12Element* back_src = move_src;
            Vector12Element* back_dst = finish;
            std::intptr_t remaining = prefix_count;

            do {
                --remaining;
                back_dst[-1] = back_src[-1];
                --back_src;
                --back_dst;
            } while (remaining != 0);
        }

        Vector12Element* const fill_end = position + count;
        for (Vector12Element* dst = position; dst != fill_end; ++dst) {
            *dst = value;
        }
    } else {
        const std::uint32_t tail_fill_count = count - trailing_count;
        count = trailing_count;
        Vector12Element* const old_finish = finish;

        std::fill_n(old_finish, tail_fill_count, value);
        self->finish = self->finish + tail_fill_count;

        Vector12Element* out = self->finish;
        for (Vector12Element* src = position; src != finish; ++src) {
            if (out != nullptr) {
                *out = *src;
                finish = old_finish;
            }
            ++out;
        }

        self->finish = self->finish + count;

        if (position != finish) {
            Vector12Element* dst = position;
            do {
                *dst = value;
                ++dst;
            } while (dst != old_finish);
        }
    }
}