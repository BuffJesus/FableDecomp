#include <cstddef>
#include <cstdint>

class CScriptThing;

struct CCharStringRep
{
    const char* m_pString;   // 0x00
    std::int32_t m_Length;   // 0x04
};

static_assert(offsetof(CCharStringRep, m_pString) == 0x00);
static_assert(offsetof(CCharStringRep, m_Length) == 0x04);
static_assert(sizeof(CCharStringRep) == 0x08);

class CCharString
{
public:
    CCharStringRep* m_pRep; // 0x00

    CCharString(const char* pText, int length);
    ~CCharString();
};

static_assert(offsetof(CCharString, m_pRep) == 0x00);
static_assert(sizeof(CCharString) == 0x04);

class CGameScriptInterface
{
public:
    void __thiscall EntitySetSleepEnabled(
        const CScriptThing& entity,
        bool enabled) const;
};

namespace
{
using CGameScriptInterface_Offset938Fn =
    void (__thiscall*)(const CGameScriptInterface*,
                       const CScriptThing&,
                       const CCharString&,
                       bool);

struct CGameScriptInterface_VTable_Overlay
{
    std::byte m_Pad00[0x938];
    CGameScriptInterface_Offset938Fn m_Offset938; // 0x938
};

static_assert(offsetof(CGameScriptInterface_VTable_Overlay, m_Offset938) == 0x938);
}

void __thiscall CGameScriptInterface::EntitySetSleepEnabled(
    const CScriptThing& entity,
    bool enabled) const
{
    CCharString script_property_name("SG_MINION_SLEEP", -1);

    const auto* const vtable =
        *reinterpret_cast<const CGameScriptInterface_VTable_Overlay* const*>(this);

    vtable->m_Offset938(this, entity, script_property_name, enabled);
}