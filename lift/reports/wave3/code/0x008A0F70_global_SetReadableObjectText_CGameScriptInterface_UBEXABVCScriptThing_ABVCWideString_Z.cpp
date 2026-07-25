#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CWideString;

enum ETCInterfaceType : std::int32_t
{
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;      // 0x00
    TValue m_Value;  // 0x04
};

static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Key) == 0x00);
static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Value) == 0x04);
static_assert(sizeof(CKeyPair<ETCInterfaceType, CTCBase*>) == 0x08);

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

static_assert(offsetof(
                  CVectorMap<
                      ETCInterfaceType,
                      CTCBase*,
                      CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>,
                  m_pBegin) == 0x00);
static_assert(offsetof(
                  CVectorMap<
                      ETCInterfaceType,
                      CTCBase*,
                      CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>,
                  m_pEnd) == 0x04);
static_assert(sizeof(
                  CVectorMap<
                      ETCInterfaceType,
                      CTCBase*,
                      CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>) == 0x08);

class CWideString
{
public:
    CWideString& __thiscall operator=(const CWideString& other);
};

class CGameScriptInterface
{
public:
    void __thiscall SetReadableObjectText(
        const CScriptThing& scriptThing,
        const CWideString& text) const;
};

namespace
{
using TCInterfaceEntry = CKeyPair<ETCInterfaceType, CTCBase*>;
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    void* (__thiscall* m_GetEntityAt2C)(const CScriptThing* pThis); // 0x2C
};

static_assert(offsetof(CScriptThingVTableOverlay, m_GetEntityAt2C) == 0x2C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct SetReadableObjectText_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_Flags20; // 0x20
    std::byte m_Pad24[0x20];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(SetReadableObjectText_TargetOverlay, m_Flags20) == 0x20);
static_assert(offsetof(SetReadableObjectText_TargetOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(SetReadableObjectText_TargetOverlay, m_Flags91) == 0x91);

struct SetReadableObjectText_InterfaceOverlay
{
    std::byte m_Pad00[0x18];
    CWideString m_Text18; // 0x18
};

static_assert(offsetof(SetReadableObjectText_InterfaceOverlay, m_Text18) == 0x18);
}

void __thiscall CGameScriptInterface::SetReadableObjectText(
    const CScriptThing& scriptThing,
    const CWideString& text) const
{
    auto* const pScriptThing = reinterpret_cast<const CScriptThingOverlay*>(&scriptThing);
    auto* const pTarget = reinterpret_cast<SetReadableObjectText_TargetOverlay*>(
        pScriptThing->m_pVTable->m_GetEntityAt2C(&scriptThing));

    if ((pTarget != nullptr) &&
        ((pTarget->m_Flags91 & 0x01) == 0) &&
        ((pTarget->m_Flags20 & 0x00002000) != 0))
    {
        const ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x0D);
        TCInterfaceEntry* pEntry = pTarget->m_InterfaceMap44.LowerBound(&interfaceType);

        if ((pEntry == pTarget->m_InterfaceMap44.m_pEnd) ||
            (static_cast<ETCInterfaceType>(0x0D) < pEntry->m_Key))
        {
            pEntry = pTarget->m_InterfaceMap44.m_pEnd;
        }

        reinterpret_cast<SetReadableObjectText_InterfaceOverlay*>(pEntry->m_Value)->m_Text18 = text;
    }
}