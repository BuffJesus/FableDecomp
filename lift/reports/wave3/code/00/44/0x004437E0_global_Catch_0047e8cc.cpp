#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CatchEntry {
    void** vftable;
    std::uint32_t unknown_04;
};

static_assert(sizeof(CatchEntry) == 0x8);
static_assert(offsetof(CatchEntry, vftable) == 0x0);
static_assert(offsetof(CatchEntry, unknown_04) == 0x4);

struct CatchOverlay {
    std::byte pad_00[0x1C];
    CatchEntry* begin;
    CatchEntry* end;
};

static_assert(offsetof(CatchOverlay, begin) == 0x1C);
static_assert(offsetof(CatchOverlay, end) == 0x20);

void __fastcall Catch_0047e8cc(void* param_1)
{
    if (param_1 != nullptr) {
        auto* self = static_cast<CatchOverlay*>(param_1);
        CatchEntry* const end = self->end;

        for (CatchEntry* it = self->begin; it != end; ++it) {
            reinterpret_cast<void(__thiscall*)(CatchEntry*, int)>(it->vftable[0])(it, 0);
        }

        if (self->begin != nullptr) {
            std::free(self->begin);
        }

        ::operator delete(param_1);
    }
}