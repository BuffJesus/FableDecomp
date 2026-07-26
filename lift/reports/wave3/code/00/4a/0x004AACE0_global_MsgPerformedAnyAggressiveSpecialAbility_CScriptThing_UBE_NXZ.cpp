#include <cstddef>
#include <cstdint>

class CScriptThing;

namespace
{
struct CScriptThing_MsgPerformedAnyAggressiveSpecialAbility_DispatchTargetOverlay
{
    void** vftable; // +0x00
};
static_assert(offsetof(CScriptThing_MsgPerformedAnyAggressiveSpecialAbility_DispatchTargetOverlay, vftable) == 0x00);

struct CScriptThing_MsgPerformedAnyAggressiveSpecialAbility_SelfOverlay
{
    void** vftable; // +0x00
    CScriptThing_MsgPerformedAnyAggressiveSpecialAbility_DispatchTargetOverlay* field04; // +0x04
};
static_assert(offsetof(CScriptThing_MsgPerformedAnyAggressiveSpecialAbility_SelfOverlay, field04) == 0x04);

using CScriptThing_MsgPerformedAnyAggressiveSpecialAbility_IndirectFn =
    bool(__thiscall*)(const CScriptThing_MsgPerformedAnyAggressiveSpecialAbility_DispatchTargetOverlay*);
}

bool __thiscall CScriptThing::MsgPerformedAnyAggressiveSpecialAbility() const
{
    const auto* const self =
        reinterpret_cast<const CScriptThing_MsgPerformedAnyAggressiveSpecialAbility_SelfOverlay*>(this);
    const auto* const target = self->field04;
    if (target == nullptr) {
        return false;
    }

    const auto fn = reinterpret_cast<CScriptThing_MsgPerformedAnyAggressiveSpecialAbility_IndirectFn>(
        *reinterpret_cast<void* const*>(reinterpret_cast<std::uintptr_t>(target->vftable) + 0x9C));
    return fn(target);
}