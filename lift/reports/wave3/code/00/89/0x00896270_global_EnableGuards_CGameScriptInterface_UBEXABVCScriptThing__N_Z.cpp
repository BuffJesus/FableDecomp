#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCVillage;
class CThing;

enum ETCInterfaceType : std::int32_t
{
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;     // 0x00
    TValue m_Value; // 0x04
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

class CTCVillage
{
public:
    void __thiscall EnableGuards(bool enabled);
};

class CGameScriptInterface
{
public:
    void __thiscall EnableGuards(const CScriptThing& thing, bool enabled) const;
};

namespace
{
using TCInterfaceRecord = CKeyPair<ETCInterfaceType, CTCBase*>;
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CThing* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CThingEnableGuardsOverlay
{
    std::byte m_Pad00[0x24];
    std::uint8_t m_Flags24; // 0x24
    std::byte m_Pad25[0x1F];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThingEnableGuardsOverlay, m_Flags24) == 0x24);
static_assert(offsetof(CThingEnableGuardsOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(CThingEnableGuardsOverlay, m_Flags91) == 0x91);

constexpr ETCInterfaceType kVillageInterfaceType = static_cast<ETCInterfaceType>(0x22);
}

void __thiscall CGameScriptInterface::EnableGuards(
    const CScriptThing& thing,
    bool enabled) const
{
    const auto* const pScriptThing = reinterpret_cast<const CScriptThingOverlay*>(&thing);
    CThing* const pThing = pScriptThing->m_pVTable->GetThing(&thing);

    if (pThing != nullptr)
    {
        auto* const pThingOverlay = reinterpret_cast<CThingEnableGuardsOverlay*>(pThing);
        if (((pThingOverlay->m_Flags91 & 0x01) == 0) &&
            ((pThingOverlay->m_Flags24 & 0x04) != 0))
        {
            TCInterfaceRecord* pRecord =
                pThingOverlay->m_InterfaceMap44.LowerBound(&kVillageInterfaceType);

            if ((pRecord == pThingOverlay->m_InterfaceMap44.m_pEnd) ||
                (kVillageInterfaceType < pRecord->m_Key))
            {
                pRecord = pThingOverlay->m_InterfaceMap44.m_pEnd;
            }

            reinterpret_cast<CTCVillage*>(pRecord->m_Value)->EnableGuards(enabled);
        }
    }
}