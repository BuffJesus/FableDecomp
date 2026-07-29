#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct Catch_0047e8cc_Overlay {
    std::byte pad_00[0x1C];
    void* begin;          // 0x1C
    void* end;            // 0x20
    std::byte pad_24[0x10];
    void* heapBlock34;    // 0x34
};

static_assert(offsetof(Catch_0047e8cc_Overlay, begin) == 0x1C);
static_assert(offsetof(Catch_0047e8cc_Overlay, end) == 0x20);
static_assert(offsetof(Catch_0047e8cc_Overlay, heapBlock34) == 0x34);

void __fastcall Catch_0047e8cc(std::intptr_t selfRaw)
{
    auto* const self = reinterpret_cast<Catch_0047e8cc_Overlay*>(selfRaw);

    if (self->heapBlock34 != nullptr) {
        std::free(self->heapBlock34);
    }

    auto* const end = reinterpret_cast<std::uint32_t*>(self->end);
    for (auto* it = reinterpret_cast<std::uint32_t*>(self->begin); it != end; it += 2) {
        using RawIndirectCall = void(__thiscall*)(void*, int);
        auto* const slot0 = reinterpret_cast<RawIndirectCall*>(*reinterpret_cast<void**>(it));
        (*slot0)(it, 0);
    }

    if (self->begin != nullptr) {
        std::free(self->begin);
    }
}