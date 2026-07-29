#include <cstddef>
#include <cstdlib>
#include <new>

struct Catch_0047e8cc_Overlay {
    std::byte pad_00[0x1C];
    void* begin;
    void* end;
};

static_assert(offsetof(Catch_0047e8cc_Overlay, begin) == 0x1C);
static_assert(offsetof(Catch_0047e8cc_Overlay, end) == 0x20);

void __fastcall Catch_0047e8cc(void* param_1)
{
    if (param_1 != nullptr) {
        auto* const self = static_cast<Catch_0047e8cc_Overlay*>(param_1);
        auto* current = static_cast<std::byte*>(self->begin);
        auto* const end = static_cast<std::byte*>(self->end);

        while (current != end) {
            const auto vftable = *reinterpret_cast<void***>(current);
            reinterpret_cast<void(__thiscall*)(void*, int)>(vftable[0])(current, 0);
            current += 8;
        }

        if (self->begin != nullptr) {
            std::free(self->begin);
        }

        ::operator delete(param_1);
    }
}