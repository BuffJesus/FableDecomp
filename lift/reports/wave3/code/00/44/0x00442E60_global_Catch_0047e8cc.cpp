#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>

struct Catch_0047e8cc_Entry {
    void* slot0;
    std::uint32_t unknown_04;
};
static_assert(sizeof(Catch_0047e8cc_Entry) == 0x08);

struct Catch_0047e8cc_Overlay {
    std::byte pad_00[0x1C];
    Catch_0047e8cc_Entry* entries_begin;
    Catch_0047e8cc_Entry* entries_end;
    CBaseIntelligentPointer pointer_28;
};

static_assert(offsetof(Catch_0047e8cc_Overlay, entries_begin) == 0x1C);
static_assert(offsetof(Catch_0047e8cc_Overlay, entries_end) == 0x20);
static_assert(offsetof(Catch_0047e8cc_Overlay, pointer_28) == 0x28);

void* __thiscall Catch_0047e8cc(void* self, std::uint8_t delete_flag)
{
    auto* const obj = static_cast<Catch_0047e8cc_Overlay*>(self);

    obj->pointer_28.~CBaseIntelligentPointer();

    using Slot0Call = void(__thiscall*)(Catch_0047e8cc_Entry*, int);

    for (Catch_0047e8cc_Entry* it = obj->entries_begin; it != obj->entries_end; ++it) {
        auto const slot0 = (*reinterpret_cast<void***>(it))[0];
        reinterpret_cast<Slot0Call>(slot0)(it, 0);
    }

    if (obj->entries_begin != nullptr) {
        std::free(obj->entries_begin);
    }

    if ((delete_flag & 1u) != 0) {
        ::operator delete(self);
    }

    return self;
}