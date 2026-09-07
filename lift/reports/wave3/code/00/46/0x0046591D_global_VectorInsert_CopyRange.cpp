#include <cstddef>
#include <cstdint>

struct VectorInsert_CopyRange_Overlay {
    std::byte pad_0[0x4];
    std::uint32_t* end;
};

static_assert(offsetof(VectorInsert_CopyRange_Overlay, end) == 0x4);

std::uint32_t* __thiscall VectorInsert_CopyRange(
    VectorInsert_CopyRange_Overlay* self,
    std::uint32_t* dst,
    std::uint32_t* srcBegin)
{
    int count =
        (static_cast<int>(reinterpret_cast<std::int32_t>(self->end)) -
         static_cast<int>(reinterpret_cast<std::int32_t>(srcBegin))) >>
        2;

    std::uint32_t* current = dst;
    if (0 < count) {
        const int sourceOffset =
            static_cast<int>(reinterpret_cast<std::int32_t>(srcBegin)) -
            static_cast<int>(reinterpret_cast<std::int32_t>(dst));

        do {
            *current = *reinterpret_cast<std::uint32_t*>(
                sourceOffset + static_cast<int>(reinterpret_cast<std::int32_t>(current)));
            ++current;
            --count;
        } while (count != 0);
    }

    self->end = current;
    return dst;
}