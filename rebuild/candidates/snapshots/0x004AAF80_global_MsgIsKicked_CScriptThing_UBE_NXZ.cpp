#include <cstddef>
#include <cstdint>

class CScriptThing;

namespace
{
struct CScriptThing_MsgIsKicked_DispatchTargetOverlay
{
    void** vftable; // +0x00
};
static_assert(offsetof(CScriptThing_MsgIsKicked_DispatchTargetOverlay, vftable) == 0x00);

struct CScriptThing_MsgIsKicked_SelfOverlay
{
    void** vftable; // +0x00
    CScriptThing_MsgIsKicked_DispatchTargetOverlay* field04; // +0x04
};
static_assert(offsetof(CScriptThing_MsgIsKicked_SelfOverlay, field04) == 0x04);

using CScriptThing_MsgIsKicked_IndirectFn =
    bool(__thiscall*)(const CScriptThing_MsgIsKicked_DispatchTargetOverlay*);
}

bool __thiscall CScriptThing::MsgIsKicked() const
{
    const auto* const self = reinterpret_cast<const CScriptThing_MsgIsKicked_SelfOverlay*>(this);
    const auto* const target = self->field04;
    if (target == nullptr) {
        return false;
    }

    const auto fn = reinterpret_cast<CScriptThing_MsgIsKicked_IndirectFn>(
        reinterpret_cast<std::uintptr_t>(target->vftable[0xF0 / sizeof(void*)]));
    return fn(target);
}