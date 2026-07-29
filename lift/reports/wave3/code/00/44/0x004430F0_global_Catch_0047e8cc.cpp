#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CCharString;

struct Catch_0047e8cc_Overlay {
    std::byte pad_00[0x1C];
    void* begin;      // 0x1C
    void* end;        // 0x20
    std::byte pad_24[0x04];
    CCharString charString; // 0x28
};

static_assert(offsetof(Catch_0047e8cc_Overlay, begin) == 0x1C);
static_assert(offsetof(Catch_0047e8cc_Overlay, end) == 0x20);
static_assert(offsetof(Catch_0047e8cc_Overlay, charString) == 0x28);

void* __thiscall Catch_0047e8cc(void* self, std::uint8_t flags)
{
    auto* obj = static_cast<Catch_0047e8cc_Overlay*>(self);

    obj->charString.~CCharString();

    auto* finish = static_cast<std::uint32_t*>(obj->end);
    for (auto* it = static_cast<std::uint32_t*>(obj->begin); it != finish; it += 2) {
        using RawSlotCall = void(__thiscall*)(void*, int);
        auto* const table = *reinterpret_cast<RawSlotCall* const*>(it);
        (*table)(it, 0);
    }

    if (obj->begin != nullptr) {
        std::free(obj->begin);
    }

    if ((flags & 1) != 0) {
        ::operator delete(self);
    }

    return self;
}