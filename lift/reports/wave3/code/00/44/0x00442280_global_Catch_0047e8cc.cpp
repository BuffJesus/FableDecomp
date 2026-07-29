#include <cstddef>
#include <cstdlib>

struct Catch0047E8CC_Element
{
    void** vftable;
    std::byte unknown_04[4];
};
static_assert(sizeof(Catch0047E8CC_Element) == 0x8);

struct Catch0047E8CC_Overlay
{
    std::byte pad_00[0x30];
    Catch0047E8CC_Element* array0_begin; // 0x30
    Catch0047E8CC_Element* array0_end;   // 0x34
    std::byte pad_38[0x4];
    Catch0047E8CC_Element* array1_begin; // 0x3C
    Catch0047E8CC_Element* array1_end;   // 0x40
};
static_assert(offsetof(Catch0047E8CC_Overlay, array0_begin) == 0x30);
static_assert(offsetof(Catch0047E8CC_Overlay, array0_end) == 0x34);
static_assert(offsetof(Catch0047E8CC_Overlay, array1_begin) == 0x3C);
static_assert(offsetof(Catch0047E8CC_Overlay, array1_end) == 0x40);

void __fastcall Catch_0047e8cc(Catch0047E8CC_Overlay* self)
{
    using Slot0Call = void(__thiscall*)(void*, int);

    Catch0047E8CC_Element* end = self->array1_end;
    for (Catch0047E8CC_Element* it = self->array1_begin; it != end; ++it)
    {
        const auto fn = reinterpret_cast<Slot0Call>(it->vftable[0]);
        fn(it, 0);
    }

    if (self->array1_begin != nullptr)
    {
        std::free(self->array1_begin);
    }

    end = self->array0_end;
    for (Catch0047E8CC_Element* it = self->array0_begin; it != end; ++it)
    {
        const auto fn = reinterpret_cast<Slot0Call>(it->vftable[0]);
        fn(it, 0);
    }

    if (self->array0_begin != nullptr)
    {
        std::free(self->array0_begin);
    }
}