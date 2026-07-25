#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CThing;
class CVillageTaskCarryCrate;

enum ETCInterfaceType : std::int32_t
{
    ETCInterfaceType_PlayerCreatureOnlyTarget = 8,
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

class CVillageTaskCarryCrate
{
public:
    void __thiscall AssignCreatureToTask(CThing* pCreature);
};

class CGameScriptInterface
{
public:
    void __thiscall SetPlayerCreatureOnlyTarget(const CScriptThing& thing) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CThing* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
    std::byte m_Pad30[0xFC];
    bool (__thiscall* IsAvailableForScript)(const CScriptThing* pThis); // 0x12C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);
static_assert(offsetof(CScriptThingVTableOverlay, IsAvailableForScript) == 0x12C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CGameScriptInterfaceResolverSourceOverlay
{
    std::byte m_Pad00[0x1C];
    void* m_Field1C; // 0x1C
};

static_assert(offsetof(CGameScriptInterfaceResolverSourceOverlay, m_Field1C) == 0x1C);

struct CGameScriptInterfaceOverlay
{
    std::byte m_Pad00[0x08];
    CGameScriptInterfaceResolverSourceOverlay* m_Field08; // 0x08
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_Field08) == 0x08);

struct CThingCreatureBaseOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_InterfaceFlags20; // 0x20
    std::byte m_Pad24[0x20];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThingCreatureBaseOverlay, m_InterfaceFlags20) == 0x20);
static_assert(offsetof(CThingCreatureBaseOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(CThingCreatureBaseOverlay, m_Flags91) == 0x91);

using ResolveField1CFn = void* (__thiscall*)(void* pValue);
using ResolveThingFn = CThingCreatureBaseOverlay* (__thiscall*)(void* pValue);

constexpr auto kResolveField1C = reinterpret_cast<ResolveField1CFn>(0x00449970);
constexpr auto kResolveThing = reinterpret_cast<ResolveThingFn>(0x00487DC0);
}

void __thiscall CGameScriptInterface::SetPlayerCreatureOnlyTarget(const CScriptThing& thing) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
    CThingCreatureBaseOverlay* const pTarget =
        kResolveThing(kResolveField1C(self->m_Field08->m_Field1C));

    const auto* const thingOverlay = reinterpret_cast<const CScriptThingOverlay*>(&thing);
    if (thingOverlay->m_pVTable->IsAvailableForScript(&thing) &&
        pTarget != nullptr &&
        ((pTarget->m_Flags91 & 0x01) == 0))
    {
        CThing* const pThing = thingOverlay->m_pVTable->GetThing(&thing);
        if ((pTarget->m_InterfaceFlags20 & 0x00000100) != 0)
        {
            ETCInterfaceType interfaceType = ETCInterfaceType_PlayerCreatureOnlyTarget;
            auto* pEntry = pTarget->m_InterfaceMap44.LowerBound(&interfaceType);

            if ((pEntry == pTarget->m_InterfaceMap44.m_pEnd) ||
                (ETCInterfaceType_PlayerCreatureOnlyTarget < pEntry->m_Key))
            {
                pEntry = pTarget->m_InterfaceMap44.m_pEnd;
            }

            reinterpret_cast<CVillageTaskCarryCrate*>(pEntry->m_Value)->AssignCreatureToTask(pThing);
        }
    }
}