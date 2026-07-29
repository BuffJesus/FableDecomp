#include <cstddef>
#include <cstdlib>
#include <new>

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
        auto* end = static_cast<std::uint32_t*>(self->end);

        for (auto* cursor = static_cast<std::uint32_t*>(self->begin); cursor != end; cursor += 2) {
            auto*** const object = reinterpret_cast<void***>(cursor);
            using Vfunc0 = void(__thiscall*)(void*, int);
            reinterpret_cast<Vfunc0>((*object)[0])(cursor, 0);
        }

        if (self->begin != nullptr) {
            std::free(self->begin);
        }

        ::operator delete(param_1);
    }
}