#include <cstddef>
#include <cstdint>

static_assert(sizeof(unsigned long) == 4);

struct VectorInsertOverlay {
    std::byte pad_00[0x4];
    unsigned long* finish;
    unsigned long* end_of_storage;

    void __thiscall Vector_Reallocate(
        unsigned long* position,
        unsigned long* value,
        unsigned char* count_high_byte,
        unsigned int count,
        int zero
    );
};

static_assert(offsetof(VectorInsertOverlay, finish) == 0x4);
static_assert(offsetof(VectorInsertOverlay, end_of_storage) == 0x8);

namespace std {
    unsigned long* _Fill_n<unsigned long*, unsigned int, unsigned long>(
        unsigned long* first,
        unsigned int count,
        const unsigned long* value
    );
}

void __thiscall Vector_Insert(unsigned long self_value, unsigned long* position, unsigned int count, unsigned long* value)
{
    auto* const self = reinterpret_cast<VectorInsertOverlay*>(static_cast<std::uintptr_t>(self_value));
    unsigned long* copy_from;
    int move_count;
    unsigned long* copy_to;
    unsigned int original_count;
    unsigned int tail_count;
    unsigned int fill_count;
    unsigned long* old_finish;
    unsigned long local_c;
    unsigned long local_8;

    original_count = count;
    if (count != 0) {
        if (static_cast<unsigned int>(
                (static_cast<int>(reinterpret_cast<std::uintptr_t>(self->end_of_storage)) -
                 static_cast<int>(reinterpret_cast<std::uintptr_t>(self->finish))) >>
                3) < count) {
            count = count & 0x00FFFFFFU;
            local_c = self_value;
            local_8 = self_value;
            self->Vector_Reallocate(position, value, reinterpret_cast<unsigned char*>(&count) + 3, original_count, 0);
        } else {
            local_c = value[0];
            local_8 = value[1];
            old_finish = self->finish;
            tail_count = static_cast<unsigned int>(
                (static_cast<int>(reinterpret_cast<std::uintptr_t>(self->finish)) -
                 static_cast<int>(reinterpret_cast<std::uintptr_t>(position))) >>
                3);

            if (count < tail_count) {
                copy_from = old_finish + count * -2;
                copy_to = old_finish;
                for (unsigned long* it = copy_from; it != old_finish; it = it + 2) {
                    if (copy_to != nullptr) {
                        copy_to[0] = it[0];
                        copy_to[1] = it[1];
                    }
                    copy_to = copy_to + 2;
                }

                self->finish = self->finish + count * 2;
                move_count = (static_cast<int>(reinterpret_cast<std::uintptr_t>(copy_from)) -
                              static_cast<int>(reinterpret_cast<std::uintptr_t>(position))) >>
                             3;
                if (0 < move_count) {
                    do {
                        move_count = move_count + -1;
                        old_finish[-2] = copy_from[-2];
                        old_finish[-1] = copy_from[-1];
                        copy_from = copy_from + -2;
                        old_finish = old_finish + -2;
                    } while (move_count != 0);
                }

                copy_to = position + count * 2;
                for (; position != copy_to; position = position + 2) {
                    position[0] = local_c;
                    position[1] = local_8;
                }
            } else {
                fill_count = count - tail_count;
                std::_Fill_n<unsigned long*, unsigned int, unsigned long>(old_finish, fill_count, &local_c);
                self->finish = self->finish + fill_count * 2;
                copy_from = self->finish;
                for (copy_to = position; copy_to != old_finish; copy_to = copy_to + 2) {
                    if (copy_from != nullptr) {
                        copy_from[0] = copy_to[0];
                        copy_from[1] = copy_to[1];
                    }
                    copy_from = copy_from + 2;
                }

                self->finish = self->finish + tail_count * 2;
                for (; position != old_finish; position = position + 2) {
                    position[0] = local_c;
                    position[1] = local_8;
                }
            }
        }
    }
}