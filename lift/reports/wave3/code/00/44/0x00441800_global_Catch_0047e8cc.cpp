#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct Catch_0047e8cc_Entry
{
    void** vftable;
    std::uint32_t reserved;
};
static_assert(sizeof(Catch_0047e8cc_Entry) == 0x8);

struct Catch_0047e8cc_Overlay
{
    std::byte pad_00[0x1C];
    Catch_0047e8cc_Entry* range0_begin; // 0x1C
    Catch_0047e8cc_Entry* range0_end;   // 0x20
    std::uint32_t unknown_24;           // 0x24
    Catch_0047e8cc_Entry* range1_begin; // 0x28
    Catch_0047e8cc_Entry* range1_end;   // 0x2C
};

static_assert(offsetof(Catch_0047e8cc_Overlay, range0_begin) == 0x1C);
static_assert(offsetof(Catch_0047e8cc_Overlay, range0_end) == 0x20);
static_assert(offsetof(Catch_0047e8cc_Overlay, unknown_24) == 0x24);
static_assert(offsetof(Catch_0047e8cc_Overlay, range1_begin) == 0x28);
static_assert(offsetof(Catch_0047e8cc_Overlay, range1_end) == 0x2C);

using Catch_0047e8cc_Slot0 = void(__thiscall*)(void* /*entry*/, int /*arg0*/);

void __fastcall Catch_0047e8cc(void* self_ptr)
{
    auto* const self = static_cast<Catch_0047e8cc_Overlay*>(self_ptr);

    Catch_0047e8cc_Entry* end = self->range1_end;
    for (Catch_0047e8cc_Entry* it = self->range1_begin; it != end; ++it) {
        const auto fn = reinterpret_cast<Catch_0047e8cc_Slot0>(it->vftable[0]);
        fn(it, 0);
    }

    if (self->range1_begin != nullptr) {
        std::free(self->range1_begin);
    }

    end = self->range0_end;
    for (Catch_0047e8cc_Entry* it = self->range0_begin; it != end; ++it) {
        const auto fn = reinterpret_cast<Catch_0047e8cc_Slot0>(it->vftable[0]);
        fn(it, 0);
    }

    if (self->range0_begin != nullptr) {
        std::free(self->range0_begin);
    }
}