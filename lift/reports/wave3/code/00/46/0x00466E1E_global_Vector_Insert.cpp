#include <cstddef>
#include <cstdint>
#include <algorithm>

struct VectorInsertValue {
    unsigned long v0;
    unsigned long v1;
    unsigned long v2;
};
static_assert(sizeof(VectorInsertValue) == 0x0C);

struct VectorInsertOverlay {
    std::byte pad_00[0x04];
    unsigned long* finish;          // +0x04
    unsigned long* end_of_storage;  // +0x08
};
static_assert(offsetof(VectorInsertOverlay, finish) == 0x04);
static_assert(offsetof(VectorInsertOverlay, end_of_storage) == 0x08);

extern void __thiscall Vector_Reallocate(
    VectorInsertOverlay* self,
    unsigned long* insert_pos,
    const unsigned long* value,
    unsigned char* count_end,
    unsigned int requested_count,
    int unused_zero);

void __thiscall Vector_Insert(
    VectorInsertOverlay* self,
    unsigned long* insert_pos,
    unsigned int count,
    const unsigned long* value)
{
    unsigned int requested_count = count;

    if (count == 0) {
        return;
    }

    if (static_cast<unsigned int>(
            (reinterpret_cast<std::intptr_t>(self->end_of_storage) -
             reinterpret_cast<std::intptr_t>(self->finish)) / 0x0C) < count) {
        unsigned int count_copy = count;
        reinterpret_cast<unsigned char*>(&count_copy)[3] = 0;
        Vector_Reallocate(
            self,
            insert_pos,
            value,
            reinterpret_cast<unsigned char*>(&count_copy) + 3,
            requested_count,
            0);
        return;
    }

    VectorInsertValue value_copy{value[0], value[1], value[2]};
    unsigned int tail_count = static_cast<unsigned int>(
        (reinterpret_cast<std::intptr_t>(self->finish) -
         reinterpret_cast<std::intptr_t>(insert_pos)) / 0x0C);
    unsigned long* finish = self->finish;

    if (count < tail_count) {
        unsigned long* src_begin = finish - count * 3;
        unsigned long* dst = finish;

        for (unsigned long* src = src_begin; src != finish; src += 3) {
            if (dst != nullptr) {
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
            }
            dst += 3;
        }

        int move_count = static_cast<int>(
            (reinterpret_cast<std::intptr_t>(src_begin) -
             reinterpret_cast<std::intptr_t>(insert_pos)) / 0x0C);

        self->finish = self->finish + count * 3;

        if (move_count > 0) {
            do {
                --move_count;
                finish[-3] = src_begin[-3];
                finish[-2] = src_begin[-2];
                finish[-1] = src_begin[-1];
                src_begin -= 3;
                finish -= 3;
            } while (move_count != 0);
        }

        unsigned long* fill_end = insert_pos + count * 3;
        for (; insert_pos != fill_end; insert_pos += 3) {
            insert_pos[0] = value_copy.v0;
            insert_pos[1] = value_copy.v1;
            insert_pos[2] = value_copy.v2;
        }
    } else {
        const unsigned int append_count = count - tail_count;
        count = tail_count;
        unsigned long* original_finish = finish;

        std::fill_n(
            reinterpret_cast<VectorInsertValue*>(finish),
            append_count,
            value_copy);

        self->finish = self->finish + append_count * 3;

        unsigned long* dst = self->finish;
        for (unsigned long* src = insert_pos; src != finish; src += 3) {
            if (dst != nullptr) {
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                finish = original_finish;
            }
            dst += 3;
        }

        self->finish = self->finish + count * 3;

        if (insert_pos != finish) {
            do {
                insert_pos[0] = value_copy.v0;
                insert_pos[1] = value_copy.v1;
                insert_pos[2] = value_copy.v2;
                insert_pos += 3;
            } while (insert_pos != original_finish);
        }
    }
}