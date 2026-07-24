#include <cstddef>

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
struct CScriptThing_GetHomeMapName_DispatchTargetOverlay
{
    void** vftable; // 0x00
};
static_assert(
    offsetof(CScriptThing_GetHomeMapName_DispatchTargetOverlay, vftable) == 0x00);

struct CScriptThing_GetHomeMapName_SelfOverlay
{
    void** vftable; // 0x00
    CScriptThing_GetHomeMapName_DispatchTargetOverlay* m_Field04; // 0x04
};
static_assert(
    offsetof(CScriptThing_GetHomeMapName_SelfOverlay, m_Field04) == 0x04);

using GetHomeMapNameDispatchFn =
    void(__thiscall*)(CScriptThing_GetHomeMapName_DispatchTargetOverlay*, CCharString*);
}

CCharString* __thiscall CScriptThing::GetHomeMapName(CCharString* pResult) const
{
    auto* const pSelf =
        reinterpret_cast<const CScriptThing_GetHomeMapName_SelfOverlay*>(this);

    if (pSelf->m_Field04 == nullptr)
    {
        CCharString::CCharString(pResult, DAT_0143e8ec);
        return pResult;
    }

    const auto dispatch =
        reinterpret_cast<GetHomeMapNameDispatchFn>(pSelf->m_Field04->vftable[9]);
    dispatch(pSelf->m_Field04, pResult);
    return pResult;
}