#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct Catch_0047e8cc_Overlay {
    std::byte pad_00[0x1C];
    void* first_begin;
    void* first_end;
    std::byte pad_24[0x04];
    void* second_begin;
    void* second_end;
};

static_assert(offsetof(Catch_0047e8cc_Overlay, first_begin) == 0x1C);
static_assert(offsetof(Catch_0047e8cc_Overlay, first_end) == 0x20);
static_assert(offsetof(Catch_0047e8cc_Overlay, second_begin) == 0x28);
static_assert(offsetof(Catch_0047e8cc_Overlay, second_end) == 0x2C);

void __fastcall Catch_0047e8cc(Catch_0047e8cc_Overlay* self) {
    using EntryCall = void(__thiscall*)(void* entry, int value);

    auto* second_end = static_cast<std::uint32_t*>(self->second_end);
    for (auto* it = static_cast<std::uint32_t*>(self->second_begin); it != second_end; it += 2) {
        auto* const vftable = *reinterpret_cast<void***>(it);
        reinterpret_cast<EntryCall>(vftable[0])(it, 0);
    }

    if (self->second_begin != nullptr) {
        std::free(self->second_begin);
    }

    auto* first_end = static_cast<std::uint32_t*>(self->first_end);
    for (auto* it = static_cast<std::uint32_t*>(self->first_begin); it != first_end; it += 2) {
        auto* const vftable = *reinterpret_cast<void***>(it);
        reinterpret_cast<EntryCall>(vftable[0])(it, 0);
    }

    if (self->first_begin != nullptr) {
        std::free(self->first_begin);
    }
}