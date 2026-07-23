#include <cstddef>
#include <cstdint>

class CScriptThing;

namespace
{
struct CScriptThing_SetAsUsable_DispatchTargetOverlay
{
    void** vftable; // +0x00
};
static_assert(offsetof(CScriptThing_SetAsUsable_DispatchTargetOverlay, vftable) == 0x00);

struct CScriptThing_SetAsUsable_SelfOverlay
{
    void** vftable; // +0x00
    CScriptThing_SetAsUsable_DispatchTargetOverlay* field04; // +0x04
};
static_assert(offsetof(CScriptThing_SetAsUsable_SelfOverlay, field04) == 0x04);

using CScriptThing_SetAsUsable_IndirectFn =
    void(__thiscall*)(CScriptThing_SetAsUsable_DispatchTargetOverlay*, bool);
static_assert(0x108 % sizeof(void*) == 0);
}

void __thiscall CScriptThing::SetAsUsable(bool usable)
{
    auto* const self = reinterpret_cast<CScriptThing_SetAsUsable_SelfOverlay*>(this);
    CScriptThing_SetAsUsable_DispatchTargetOverlay* const target = self->field04;
    if (target != nullptr)
    {
        const auto fn = reinterpret_cast<CScriptThing_SetAsUsable_IndirectFn>(target->vftable[0x108 / sizeof(void*)]);
        fn(target, usable);
    }
}