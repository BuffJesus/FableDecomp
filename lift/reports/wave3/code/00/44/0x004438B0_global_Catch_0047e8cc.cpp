#include <cstddef>
#include <cstdlib>

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
        auto* self = static_cast<Catch_0047e8cc_Overlay*>(param_1);
        auto* current = static_cast<std::uint32_t*>(self->begin);
        auto* finish = static_cast<std::uint32_t*>(self->end);

        while (current != finish) {
            using IndirectThiscall = void(__thiscall*)(void*, int);
            auto* const vftable = *reinterpret_cast<void***>(current);
            auto const fn = reinterpret_cast<IndirectThiscall>(vftable[0]);
            fn(current, 0);
            current += 2;
        }

        if (self->begin != nullptr) {
            std::free(self->begin);
        }

        operator delete(param_1);
    }
}