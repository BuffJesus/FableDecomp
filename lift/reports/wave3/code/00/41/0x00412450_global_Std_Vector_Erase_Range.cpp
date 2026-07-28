#include <cstddef>
#include <cstdint>

struct StdVectorEraseRangeOverlay {
    std::byte unknown_0x00[0x4];
    CWideString* end;
};

static_assert(offsetof(StdVectorEraseRangeOverlay, end) == 0x4);
static_assert(sizeof(CWideString) == 0x4);

CWideString* __thiscall Std_Vector_Erase_Range(
    StdVectorEraseRangeOverlay* self,
    CWideString* first,
    CWideString* last)
{
    int remaining = (static_cast<int>(reinterpret_cast<std::intptr_t>(self->end)) -
                     static_cast<int>(reinterpret_cast<std::intptr_t>(last))) >>
                    2;

    CWideString* current = first;
    if (remaining > 0) {
        const std::intptr_t shift =
            reinterpret_cast<std::intptr_t>(last) - reinterpret_cast<std::intptr_t>(first);
        do {
            CWideString::operator=(
                current,
                reinterpret_cast<CWideString*>(reinterpret_cast<std::intptr_t>(current) + shift));
            current = reinterpret_cast<CWideString*>(reinterpret_cast<std::intptr_t>(current) + 0x4);
            remaining = remaining - 1;
        } while (remaining != 0);
    }

    CWideString* end = self->end;
    CWideString* destroy = current;
    if (current == end) {
        self->end = current;
        return first;
    }

    do {
        destroy->CCharString::~CCharString();
        destroy = reinterpret_cast<CWideString*>(reinterpret_cast<std::intptr_t>(destroy) + 0x4);
    } while (destroy != end);

    self->end = current;
    return first;
}