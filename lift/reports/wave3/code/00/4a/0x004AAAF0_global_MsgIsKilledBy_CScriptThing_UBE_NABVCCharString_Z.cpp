#include <cstddef>

class CCharString;
class CScriptThing;

namespace
{
struct CScriptThing_MsgIsKilledBy_DispatchTargetOverlay
{
    void** vftable; // +0x00
};
static_assert(offsetof(CScriptThing_MsgIsKilledBy_DispatchTargetOverlay, vftable) == 0x00);

struct CScriptThing_MsgIsKilledBy_SelfOverlay
{
    void** vftable; // +0x00
    CScriptThing_MsgIsKilledBy_DispatchTargetOverlay* field04; // +0x04
};
static_assert(offsetof(CScriptThing_MsgIsKilledBy_SelfOverlay, field04) == 0x04);

using CScriptThing_MsgIsKilledBy_IndirectFn =
    bool(__thiscall*)(const CScriptThing_MsgIsKilledBy_DispatchTargetOverlay*, const CCharString&);
}

class CScriptThing
{
public:
    bool __thiscall MsgIsKilledBy(const CCharString& killer) const;
};

bool __thiscall CScriptThing::MsgIsKilledBy(const CCharString& killer) const
{
    const auto* const self =
        reinterpret_cast<const CScriptThing_MsgIsKilledBy_SelfOverlay*>(this);
    const auto* const target = self->field04;
    if (target == nullptr)
    {
        return false;
    }

    const auto fn = reinterpret_cast<CScriptThing_MsgIsKilledBy_IndirectFn>(
        target->vftable[0x48 / sizeof(void*)]);
    return fn(target, killer);
}