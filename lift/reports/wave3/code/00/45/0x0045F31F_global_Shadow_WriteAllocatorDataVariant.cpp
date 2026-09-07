#include <bit>
#include <cstddef>
#include <cstdint>

struct ShadowAllocatorAreaOverlay {
    std::byte _pad0[0x10];
    std::uint32_t value10;
    std::uint32_t value14;
};
static_assert(offsetof(ShadowAllocatorAreaOverlay, value10) == 0x10);
static_assert(offsetof(ShadowAllocatorAreaOverlay, value14) == 0x14);

struct ShadowAllocatorListOverlay {
    std::byte _pad0[0x08];
    ShadowAllocatorAreaOverlay* first;
};
static_assert(offsetof(ShadowAllocatorListOverlay, first) == 0x08);

struct ShadowAllocatorDataVariantOverlay {
    ShadowAllocatorListOverlay* list;
    std::uint32_t headerValue;
};
static_assert(offsetof(ShadowAllocatorDataVariantOverlay, list) == 0x00);
static_assert(offsetof(ShadowAllocatorDataVariantOverlay, headerValue) == 0x04);

void __fastcall Shadow_WriteAllocatorDataVariant(
    CDataOutputStream* stream,
    ShadowAllocatorDataVariantOverlay* allocatorData)
{
    stream->WriteCBYTE(static_cast<char>(allocatorData->headerValue));

    ShadowAllocatorAreaOverlay* area = allocatorData->list->first;
    if (area != reinterpret_cast<ShadowAllocatorAreaOverlay*>(allocatorData->list)) {
        using WriteEntryValue14Fn = void(__thiscall*)(CDataOutputStream*, float);
        using GetNoAllocatedAreasFn = void*(__cdecl*)(void*);

        static constexpr WriteEntryValue14Fn WriteEntryValue14 =
            reinterpret_cast<WriteEntryValue14Fn>(0x00993EE0);
        static constexpr GetNoAllocatedAreasFn GetNoAllocatedAreas =
            reinterpret_cast<GetNoAllocatedAreasFn>(0x004292C0);

        do {
            stream->WriteCBYTE(static_cast<char>(area->value10));
            WriteEntryValue14(stream, std::bit_cast<float>(area->value14));
            area = static_cast<ShadowAllocatorAreaOverlay*>(GetNoAllocatedAreas(area));
        } while (area != reinterpret_cast<ShadowAllocatorAreaOverlay*>(allocatorData->list));
    }
}