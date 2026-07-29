#include <cstddef>
#include <cstring>
#include <algorithm>

struct VectorInsertWithFillOverlay {
    std::byte _pad0[0x4];
    ENavigatorType* _Mylast;   // +0x04
    ENavigatorType* _Myend;    // +0x08
};

static_assert(offsetof(VectorInsertWithFillOverlay, _Mylast) == 0x4);
static_assert(offsetof(VectorInsertWithFillOverlay, _Myend) == 0x8);

extern void __cdecl Std_Vector_Insert_Range(
    ENavigatorType* insert_pos,
    ENavigatorType* value_ptr,
    ENavigatorType** value_ptr_end,
    unsigned int count,
    int reserved
);

void __thiscall vector_insert_with_fill(
    void* this_ptr,
    ENavigatorType* insert_pos,
    unsigned int count,
    ENavigatorType* value_ptr
) {
    auto* const self = static_cast<VectorInsertWithFillOverlay*>(this_ptr);

    if (count != 0) {
        if (static_cast<unsigned int>((self->_Myend - self->_Mylast)) < count) {
            Std_Vector_Insert_Range(insert_pos, value_ptr, &value_ptr, count, 0);
            return;
        }

        const ENavigatorType fill_value = *value_ptr;
        ENavigatorType* const old_last = self->_Mylast;
        const unsigned int tail_count = static_cast<unsigned int>(old_last - insert_pos);

        if (tail_count <= count) {
            const int extra_count = static_cast<int>(count - tail_count);
            int remaining = extra_count;
            ENavigatorType* write = old_last;

            if (extra_count != 0) {
                for (; remaining != 0; --remaining) {
                    *write = fill_value;
                    ++write;
                }
            }

            void* const new_last_after_extra =
                static_cast<void*>(reinterpret_cast<char*>(self->_Mylast) + extra_count * 4);
            self->_Mylast = static_cast<ENavigatorType*>(new_last_after_extra);

            if (old_last != insert_pos) {
                std::memmove(
                    new_last_after_extra,
                    insert_pos,
                    reinterpret_cast<char*>(old_last) - reinterpret_cast<char*>(insert_pos)
                );
            }

            self->_Mylast =
                reinterpret_cast<ENavigatorType*>(reinterpret_cast<char*>(self->_Mylast) + tail_count * 4);

            std::fill(insert_pos, old_last, fill_value);
            return;
        }

        ENavigatorType* const move_src = old_last - count;
        if (old_last != move_src) {
            std::memmove(
                old_last,
                move_src,
                reinterpret_cast<char*>(old_last) - reinterpret_cast<char*>(move_src)
            );
        }

        const std::size_t move_size =
            static_cast<std::size_t>(reinterpret_cast<char*>(move_src) - reinterpret_cast<char*>(insert_pos));

        self->_Mylast =
            reinterpret_cast<ENavigatorType*>(reinterpret_cast<char*>(self->_Mylast) + count * 4);

        if (static_cast<int>(move_size) > 0) {
            std::memmove(
                reinterpret_cast<void*>(reinterpret_cast<char*>(old_last) - move_size),
                insert_pos,
                move_size
            );
        }

        std::fill(insert_pos, insert_pos + count, fill_value);
    }
}