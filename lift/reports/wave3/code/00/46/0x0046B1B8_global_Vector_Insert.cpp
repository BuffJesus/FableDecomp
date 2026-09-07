#include <algorithm>
#include <cstddef>
#include <cstdint>

static_assert(sizeof(unsigned long) == 4);

struct VectorInsertOverlay {
    unsigned long* begin;
    unsigned long* end;
    unsigned long* capacity_end;
};

static_assert(offsetof(VectorInsertOverlay, begin) == 0x0);
static_assert(offsetof(VectorInsertOverlay, end) == 0x4);
static_assert(offsetof(VectorInsertOverlay, capacity_end) == 0x8);
static_assert(sizeof(VectorInsertOverlay) == 0xC);

struct VectorInsertPair {
    unsigned long first;
    unsigned long second;
};

static_assert(sizeof(VectorInsertPair) == 0x8);

void __thiscall Vector_Reallocate(
    VectorInsertOverlay* self,
    unsigned long* insert_pos,
    unsigned long* value_ptr,
    void* count_high_byte,
    unsigned int requested_count,
    int zero);

void __thiscall Vector_Insert(
    VectorInsertOverlay* self,
    unsigned long* insert_pos,
    unsigned int count,
    unsigned long* value_ptr)
{
    unsigned int requested_count = count;
    if (count != 0) {
        if (static_cast<unsigned int>((self->capacity_end - self->end) >> 1) < count) {
            count &= 0x00FFFFFFu;

            // Matches the observed `&count + 3` stack argument shape.
            Vector_Reallocate(
                self,
                insert_pos,
                value_ptr,
                reinterpret_cast<unsigned char*>(&count) + 3,
                requested_count,
                0);
        } else {
            const VectorInsertPair value{value_ptr[0], value_ptr[1]};
            unsigned long* old_end = self->end;
            const unsigned int tail_count =
                static_cast<unsigned int>((self->end - insert_pos) >> 1);

            if (count < tail_count) {
                unsigned long* src = old_end - (count * 2);
                unsigned long* dst = old_end;

                for (unsigned long* it = src; it != old_end; it += 2) {
                    if (dst != nullptr) {
                        dst[0] = it[0];
                        dst[1] = it[1];
                    }
                    dst += 2;
                }

                self->end += count * 2;

                int move_count = static_cast<int>((src - insert_pos) >> 1);
                if (move_count > 0) {
                    do {
                        --move_count;
                        old_end[-2] = src[-2];
                        old_end[-1] = src[-1];
                        src -= 2;
                        old_end -= 2;
                    } while (move_count != 0);
                }

                for (unsigned long* it = insert_pos, *stop = insert_pos + (count * 2); it != stop; it += 2) {
                    it[0] = value.first;
                    it[1] = value.second;
                }
            } else {
                const unsigned int append_count = count - tail_count;

                std::fill_n(
                    reinterpret_cast<VectorInsertPair*>(old_end),
                    append_count,
                    value);

                self->end += append_count * 2;

                unsigned long* dst = self->end;
                for (unsigned long* src = insert_pos; src != old_end; src += 2) {
                    if (dst != nullptr) {
                        dst[0] = src[0];
                        dst[1] = src[1];
                    }
                    dst += 2;
                }

                self->end += tail_count * 2;

                for (unsigned long* it = insert_pos; it != old_end; it += 2) {
                    it[0] = value.first;
                    it[1] = value.second;
                }
            }
        }
    }
}