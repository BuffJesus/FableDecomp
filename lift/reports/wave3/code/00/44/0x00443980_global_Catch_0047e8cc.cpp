#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>

struct Catch_0047e8cc_Entry {
    void** vftable;
    std::uint32_t unknown_04;
};

static_assert(sizeof(Catch_0047e8cc_Entry) == 0x8);
static_assert(offsetof(Catch_0047e8cc_Entry, vftable) == 0x0);
static_assert(offsetof(Catch_0047e8cc_Entry, unknown_04) == 0x4);

struct Catch_0047e8cc_Overlay {
    std::byte pad_00[0x1C];
    Catch_0047e8cc_Entry* begin;
    Catch_0047e8cc_Entry* end;
};

static_assert(offsetof(Catch_0047e8cc_Overlay, begin) == 0x1C);
static_assert(offsetof(Catch_0047e8cc_Overlay, end) == 0x20);

void __fastcall Catch_0047e8cc(void* param_1)
{
    if (param_1 != nullptr) {
        auto* self = static_cast<Catch_0047e8cc_Overlay*>(param_1);
        Catch_0047e8cc_Entry* const finish = self->end;
        Catch_0047e8cc_Entry* current = self->begin;

        while (current != finish) {
            auto raw_indirect = reinterpret_cast<void(__thiscall**)(void*, int)>(current->vftable);
            (*raw_indirect)(current, 0);
            current = reinterpret_cast<Catch_0047e8cc_Entry*>(
                reinterpret_cast<std::byte*>(current) + 0x8
            );
        }

        if (self->begin != nullptr) {
            std::free(self->begin);
        }

        ::operator delete(param_1);
    }
}