#include <cstddef>
#include <cstdlib>

struct Catch_0047e8cc_Entry
{
    void** vfptr;
    std::byte gap_04[4];
};
static_assert(sizeof(Catch_0047e8cc_Entry) == 0x8);

struct Catch_0047e8cc_Overlay
{
    std::byte gap_00[0x1C];
    Catch_0047e8cc_Entry* begin;
    Catch_0047e8cc_Entry* end;
};
static_assert(offsetof(Catch_0047e8cc_Overlay, begin) == 0x1C);
static_assert(offsetof(Catch_0047e8cc_Overlay, end) == 0x20);

void __fastcall Catch_0047e8cc(void* param_1)
{
    if (param_1 != nullptr)
    {
        auto* self = static_cast<Catch_0047e8cc_Overlay*>(param_1);
        Catch_0047e8cc_Entry* puVar1 = self->end;

        for (Catch_0047e8cc_Entry* puVar2 = self->begin; puVar2 != puVar1; ++puVar2)
        {
            using IndirectCall = void(__thiscall*)(void*, int);
            const auto fn = reinterpret_cast<IndirectCall>(puVar2->vfptr[0]);
            fn(puVar2, 0);
        }

        if (self->begin != nullptr)
        {
            std::free(self->begin);
        }

        ::operator delete(param_1);
    }
}