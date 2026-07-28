#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>

using ENavigatorType = std::uint32_t;

struct StdVector4ByteOverlay {
    std::byte pad_00[0x04];
    ENavigatorType* finish;         // +0x04
    ENavigatorType* end_of_storage; // +0x08
};

static_assert(offsetof(StdVector4ByteOverlay, finish) == 0x04);
static_assert(offsetof(StdVector4ByteOverlay, end_of_storage) == 0x08);

void __thiscall Std_Vector_Insert_Range_Copies_4byte(
    void* self,
    ENavigatorType* insert_at,
    ENavigatorType* value_source,
    ENavigatorType** value_source_local,
    std::uint32_t count,
    int unused_zero);

void __thiscall Std_Vector_Insert_Range_4Bytes(
    void* self,
    ENavigatorType* insert_at,
    std::uint32_t count,
    ENavigatorType* value_source)
{
    if (count == 0) {
        return;
    }

    auto* const vec = static_cast<StdVector4ByteOverlay*>(self);

    if (static_cast<std::uint32_t>((vec->end_of_storage - vec->finish)) < count) {
        ENavigatorType* value_source_local = value_source;
        Std_Vector_Insert_Range_Copies_4byte(self, insert_at, value_source, &value_source_local, count, 0);
        return;
    }

    ENavigatorType value = *value_source;
    const std::uint32_t tail_count = static_cast<std::uint32_t>(vec->finish - insert_at);
    ENavigatorType* const old_finish = vec->finish;

    if (tail_count <= count) {
        std::fill_n(old_finish, count - tail_count, value);
        vec->finish = old_finish + (count - tail_count);

        std::uninitialized_copy(insert_at, old_finish, vec->finish);
        vec->finish = vec->finish + tail_count;

        std::fill(insert_at, old_finish, value);
        return;
    }

    ENavigatorType* const src_begin = old_finish - count;
    std::uninitialized_copy(src_begin, old_finish, old_finish);
    vec->finish = vec->finish + count;

    int move_count = static_cast<int>(src_begin - insert_at);
    if (0 < move_count) {
        ENavigatorType* write = old_finish;
        do {
            ENavigatorType* const read =
                reinterpret_cast<ENavigatorType*>(
                    reinterpret_cast<std::uintptr_t>(src_begin) +
                    (-4 - reinterpret_cast<std::intptr_t>(old_finish)) +
                    reinterpret_cast<std::intptr_t>(write));
            --write;
            --move_count;
            *write = *read;
        } while (move_count != 0);
    }

    std::fill(insert_at, insert_at + count, value);
}