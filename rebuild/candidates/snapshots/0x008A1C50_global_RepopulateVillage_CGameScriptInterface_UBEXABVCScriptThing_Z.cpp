#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CThing;
class CWorld;
class CWorldMap;

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
    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

class CWorldMap
{
public:
    void __thiscall PopulateRegionWithVillagers(CThing* pVillageThing);
};

class CGameScriptInterface
{
public:
    void __thiscall RepopulateVillage(const CScriptThing& thing) const;
};

namespace
{
using TCInterfaceRecord = CKeyPair<ETCInterfaceType, CTCBase*>;
using TCInterfaceMapLowerBound =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CThing* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
    std::byte m_Pad30[0xFC];
    char (__thiscall* IsValidThing)(const CScriptThing* pThis); // 0x12C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);
static_assert(offsetof(CScriptThingVTableOverlay, IsValidThing) == 0x12C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CWorldVTableOverlay
{
    std::byte m_Pad00[0x30];
    CWorldMap* (__thiscall* GetWorldMap)(CWorld* pThis); // 0x30
};

static_assert(offsetof(CWorldVTableOverlay, GetWorldMap) == 0x30);

struct CWorldOverlay
{
    const CWorldVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CWorldOverlay, m_pVTable) == 0x00);

struct CGameScriptInterfaceOverlay
{
    std::byte m_Pad00[0x04];
    CWorld* m_pWorld; // 0x04
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_pWorld) == 0x04);

struct TCInterfaceMapOverlay
{
    std::byte m_Pad00[0x04];
    TCInterfaceRecord* m_pRecord48; // +0x04 relative to map, +0x48 in CThing
};

static_assert(offsetof(TCInterfaceMapOverlay, m_pRecord48) == 0x04);

struct VillageThingOverlay
{
    std::byte m_Pad00[0x24];
    std::uint8_t m_Flags24; // 0x24
    std::byte m_Pad25[0x1F];
    TCInterfaceMapOverlay m_InterfaceMap44; // 0x44
};

static_assert(offsetof(VillageThingOverlay, m_Flags24) == 0x24);
static_assert(offsetof(VillageThingOverlay, m_InterfaceMap44) == 0x44);

constexpr ETCInterfaceType kVillageInterfaceType = static_cast<ETCInterfaceType>(0x22);
}

void __thiscall CGameScriptInterface::RepopulateVillage(const CScriptThing& thing) const
{
    const auto* const pScriptThing = reinterpret_cast<const CScriptThingOverlay*>(&thing);
    if (pScriptThing->m_pVTable->IsValidThing(&thing) != '\0')
    {
        CThing* const pThing = pScriptThing->m_pVTable->GetThing(&thing);
        auto* const pVillageThing = reinterpret_cast<VillageThingOverlay*>(pThing);
        if ((pVillageThing->m_Flags24 & 0x04) != 0)
        {
            TCInterfaceRecord* pRecord =
                reinterpret_cast<TCInterfaceMapLowerBound*>(&pVillageThing->m_InterfaceMap44)
                    ->LowerBound(&kVillageInterfaceType);

            if ((pRecord == pVillageThing->m_InterfaceMap44.m_pRecord48) ||
                (kVillageInterfaceType < pRecord->m_Key))
            {
                pRecord = pVillageThing->m_InterfaceMap44.m_pRecord48;
            }

            if (pRecord->m_Value != nullptr)
            {
                const auto* const pInterface = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
                auto* const pWorld = reinterpret_cast<CWorldOverlay*>(pInterface->m_pWorld);
                CWorldMap* const pWorldMap = pWorld->m_pVTable->GetWorldMap(pInterface->m_pWorld);
                pWorldMap->PopulateRegionWithVillagers(pThing);
            }
        }
    }
}