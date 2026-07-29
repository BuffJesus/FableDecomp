#include <cstddef>
#include <cstdlib>

struct Catch_0047e8cc_Overlay {
    std::byte _pad0[0x1C];
    void* begin;
    void* end;
};

static_assert(offsetof(Catch_0047e8cc_Overlay, begin) == 0x1C);
static_assert(offsetof(Catch_0047e8cc_Overlay, end) == 0x20);

void __fastcall Catch_0047e8cc(void* param_1)
{
    if (param_1 != nullptr) {
        auto* const self = static_cast<Catch_0047e8cc_Overlay*>(param_1);
        auto* current = static_cast<unsigned char*>(self->begin);
        auto* const finish = static_cast<unsigned char*>(self->end);

        for (; current != finish; current += 8) {
            using RawIndirectCall = void(__thiscall*)(void*, int);
            auto** const vtable = *reinterpret_cast<RawIndirectCall***>(current);
            (*vtable[0])(current, 0);
        }

        if (self->begin != nullptr) {
            std::free(self->begin);
        }

        ::operator delete(param_1);
    }
}