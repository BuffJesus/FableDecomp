#include <cstddef>
#include <cstdint>

class CScriptThing;

namespace
{
struct CScriptThing_MsgPerformedAnyNonAggressiveSpecialAbility_DispatchTargetOverlay
{
    void** vftable; // +0x00
};
static_assert(offsetof(CScriptThing_MsgPerformedAnyNonAggressiveSpecialAbility_DispatchTargetOverlay, vftable) == 0x00);

struct CScriptThing_MsgPerformedAnyNonAggressiveSpecialAbility_SelfOverlay
{
    void** vftable; // +0x00
    CScriptThing_MsgPerformedAnyNonAggressiveSpecialAbility_DispatchTargetOverlay* field04; // +0x04
};
static_assert(offsetof(CScriptThing_MsgPerformedAnyNonAggressiveSpecialAbility_SelfOverlay, field04) == 0x04);

using CScriptThing_MsgPerformedAnyNonAggressiveSpecialAbility_IndirectFn =
    bool(__thiscall*)(const CScriptThing_MsgPerformedAnyNonAggressiveSpecialAbility_DispatchTargetOverlay*);
}

bool __thiscall CScriptThing::MsgPerformedAnyNonAggressiveSpecialAbility() const
{
    const auto* const self =
        reinterpret_cast<const CScriptThing_MsgPerformedAnyNonAggressiveSpecialAbility_SelfOverlay*>(this);
    const auto* const target = self->field04;
    if (target == nullptr) {
        return false;
    }

    const auto fn =
        reinterpret_cast<CScriptThing_MsgPerformedAnyNonAggressiveSpecialAbility_IndirectFn>(
            *reinterpret_cast<void* const*>(reinterpret_cast<std::uintptr_t>(target->vftable) + 0xA0));
    return fn(target);
}