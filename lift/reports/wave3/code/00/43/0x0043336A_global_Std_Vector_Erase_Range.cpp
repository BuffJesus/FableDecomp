#include <cstddef>
#include <cstdint>

struct CCharString;

struct StdVectorEraseRangeOverlay {
    std::uint8_t pad_0[0x4];
    CCharString* end;
};
static_assert(offsetof(StdVectorEraseRangeOverlay, end) == 0x4);

CCharString* __thiscall Std_Vector_Erase_Range(
    StdVectorEraseRangeOverlay* self,
    CCharString* first,
    CCharString* last)
{
    int count = (static_cast<int>(reinterpret_cast<std::uintptr_t>(self->end)) -
                 static_cast<int>(reinterpret_cast<std::uintptr_t>(last))) >> 2;
    CCharString* current = first;

    if (0 < count) {
        const int delta = static_cast<int>(reinterpret_cast<std::uintptr_t>(last)) -
                          static_cast<int>(reinterpret_cast<std::uintptr_t>(first));
        int remaining = count;

        do {
            current->operator=(
                *reinterpret_cast<CCharString*>(
                    reinterpret_cast<std::uintptr_t>(current) + delta));
            current = reinterpret_cast<CCharString*>(
                reinterpret_cast<std::uintptr_t>(current) + 4);
            remaining = remaining + -1;
        } while (remaining != 0);
    }

    std::_Destroy_range(current, self->end);
    self->end = current;
    return first;
}