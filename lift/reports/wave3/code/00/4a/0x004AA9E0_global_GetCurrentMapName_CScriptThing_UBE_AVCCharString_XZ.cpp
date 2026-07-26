#include <cstddef>
#include <memory>

class CCharString;
class CScriptThing;

extern CCharString DAT_0143e8ec;

class CCharString
{
public:
    CCharString(const CCharString& other);
};

namespace
{
struct CScriptThing_GetCurrentMapName_DispatchTargetOverlay
{
    void** vftable; // 0x00
};
static_assert(
    offsetof(CScriptThing_GetCurrentMapName_DispatchTargetOverlay, vftable) == 0x00);

struct CScriptThing_GetCurrentMapName_SelfOverlay
{
    void** vftable; // 0x00
    CScriptThing_GetCurrentMapName_DispatchTargetOverlay* m_Field04; // 0x04
};
static_assert(
    offsetof(CScriptThing_GetCurrentMapName_SelfOverlay, m_Field04) == 0x04);
}

class CScriptThing
{
public:
    CCharString* __thiscall GetCurrentMapName(CCharString* out) const;
};

CCharString* __thiscall CScriptThing::GetCurrentMapName(CCharString* out) const
{
    const auto* const self =
        reinterpret_cast<const CScriptThing_GetCurrentMapName_SelfOverlay*>(this);

    if (self->m_Field04 == nullptr)
    {
        std::construct_at(out, DAT_0143e8ec);
        return out;
    }

    using RawGetCurrentMapNameFn =
        void(__thiscall*)(CScriptThing_GetCurrentMapName_DispatchTargetOverlay*, CCharString*);

    auto* const vftable = self->m_Field04->vftable;
    reinterpret_cast<RawGetCurrentMapNameFn>(vftable[0x08])(self->m_Field04, out);
    return out;
}