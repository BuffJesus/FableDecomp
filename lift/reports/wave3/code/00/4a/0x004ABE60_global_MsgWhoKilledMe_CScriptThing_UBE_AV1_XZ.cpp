#include <cstddef>

class CScriptThing;

namespace
{
struct CScriptThing_MsgWhoKilledMe_DispatchTargetOverlay
{
    void** vftable; // +0x00
};
static_assert(offsetof(CScriptThing_MsgWhoKilledMe_DispatchTargetOverlay, vftable) == 0x00);

struct CScriptThing_MsgWhoKilledMe_SelfOverlay
{
    void** vftable; // +0x00
    CScriptThing_MsgWhoKilledMe_DispatchTargetOverlay* field04; // +0x04
};
static_assert(offsetof(CScriptThing_MsgWhoKilledMe_SelfOverlay, field04) == 0x04);

using CScriptThing_MsgWhoKilledMe_IndirectFn =
    void(__thiscall*)(CScriptThing_MsgWhoKilledMe_DispatchTargetOverlay*, CScriptThing*);
}

class CScriptThing
{
public:
    CScriptThing(const CScriptThing&);
};

CScriptThing* __thiscall MsgWhoKilledMe(const CScriptThing* self, CScriptThing* result)
{
    const auto* const selfOverlay =
        reinterpret_cast<const CScriptThing_MsgWhoKilledMe_SelfOverlay*>(self);

    if (selfOverlay->field04 == nullptr)
    {
        CScriptThing::CScriptThing(result, self);
        return result;
    }

    const auto fn = reinterpret_cast<CScriptThing_MsgWhoKilledMe_IndirectFn>(
        selfOverlay->field04->vftable[0x4C / sizeof(void*)]);
    fn(selfOverlay->field04, result);
    return result;
}