#include <cstddef>
#include <cstdint>

struct VectorEraseElement16
{
    std::uint32_t field_00;
    std::uint32_t field_04;
    std::uint32_t field_08;
    std::uint32_t field_0C;
};

static_assert(sizeof(VectorEraseElement16) == 0x10);

struct VectorEraseOverlay
{
    std::byte pad_00[0x4];
    VectorEraseElement16* end;
};

static_assert(offsetof(VectorEraseOverlay, end) == 0x4);

VectorEraseElement16* __thiscall Vector_Erase(
    VectorEraseOverlay* self,
    VectorEraseElement16* first,
    const VectorEraseElement16* last)
{
    int remaining =
        (static_cast<int>(reinterpret_cast<std::intptr_t>(self->end)) -
         static_cast<int>(reinterpret_cast<std::intptr_t>(last))) >>
        4;

    VectorEraseElement16* current = first;
    if (remaining > 0) {
        do {
            auto* const source = reinterpret_cast<VectorEraseElement16*>(
                (static_cast<int>(reinterpret_cast<std::intptr_t>(last)) -
                 static_cast<int>(reinterpret_cast<std::intptr_t>(first))) +
                static_cast<int>(reinterpret_cast<std::intptr_t>(current)));

            std::uint32_t* const fourthDword = reinterpret_cast<std::uint32_t*>(current) + 3;

            current->field_00 = source->field_00;
            current->field_04 = source->field_04;
            current->field_08 = source->field_08;

            current = reinterpret_cast<VectorEraseElement16*>(
                static_cast<int>(reinterpret_cast<std::intptr_t>(current)) + 0x10);
            remaining = remaining - 1;

            *fourthDword = source->field_0C;
        } while (remaining != 0);
    }

    self->end = current;
    return first;
}