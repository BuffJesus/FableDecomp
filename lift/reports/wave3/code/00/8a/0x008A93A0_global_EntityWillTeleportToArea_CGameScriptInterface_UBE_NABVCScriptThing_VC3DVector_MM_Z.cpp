#include <cstddef>
#include <cstdint>
#include <cstdlib>

class CCreatureActionBase;
class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCCreatureNavigation;
class CTCShapeManager;
class CThingCreatureBase;
class CThingSearchTools;
class CVertexBufferWin32;
class CWorld;
class CWorldMap;

struct C3DVector
{
    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};

static_assert(sizeof(C3DVector) == 0x0C);

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

class CThingCreatureBase
{
public:
    bool __thiscall SetCurrentAction(CCreatureActionBase* pAction);
};

class CVertexBufferWin32
{
public:
    static C3DVector* __thiscall DoSizeof(CVertexBufferWin32* pThis);
};

class CTCCreatureNavigation
{
public:
    static bool __thiscall IsAbleToNavigateFromPosToPos(
        CTCCreatureNavigation* pThis,
        C3DVector* pFrom,
        C3DVector* pTo,
        float zero);
};

class CGameScriptInterface
{
public:
    bool __thiscall EntityWillTeleportToArea(
        const CScriptThing& entity,
        C3DVector teleportPosition,
        float nearbyDistance,
        float searchRadius) const;
};

namespace NCreatureHiding
{
void GetValidPositions(
    void* pOutVector,
    CWorldMap* pWorldMap,
    CThingSearchTools* pThingSearchTools,
    const C3DVector* pPosition,
    float radius,
    bool allowOccupied,
    CTCShapeManager* pShapeManager);
}

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CVertexBufferWin32* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
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

struct CWorldVTableOverlay
{
    std::byte m_Pad00[0x30];
    CWorldMap* (__thiscall* GetWorldMap)(CWorld* pThis); // 0x30
};

static_assert(offsetof(CWorldVTableOverlay, GetWorldMap) == 0x30);

struct CWorldOverlay
{
    const CWorldVTableOverlay* m_pVTable; // 0x00
    std::byte m_Pad04[0x1C];
    CThingSearchTools* m_pThingSearchTools; // 0x20
};

static_assert(offsetof(CWorldOverlay, m_pThingSearchTools) == 0x20);

struct CGameScriptInterfaceOverlay
{
    std::byte m_Pad00[0x04];
    CWorld* m_pWorld; // 0x04
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_pWorld) == 0x04);

struct CThingCreatureBaseOverlay
{
    std::byte m_Pad00[0x20];
    std::int8_t m_InterfaceFlags20; // 0x20
    std::byte m_Pad21[0x23];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x20];
    std::uint8_t m_Flags6C; // 0x6C
};

static_assert(offsetof(CThingCreatureBaseOverlay, m_InterfaceFlags20) == 0x20);
static_assert(offsetof(CThingCreatureBaseOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(CThingCreatureBaseOverlay, m_Flags6C) == 0x6C);

struct C3DVectorBuffer
{
    C3DVector* m_pBegin;       // 0x00
    C3DVector* m_pEnd;         // 0x04
    C3DVector* m_pCapacityEnd; // 0x08
};

static_assert(sizeof(C3DVectorBuffer) == 0x0C);

struct CCreatureAction_TeleportOut_Storage
{
    std::byte m_Bytes[0x124];
};

static_assert(sizeof(CCreatureAction_TeleportOut_Storage) == 0x124);

using CopyImplFilteredFn =
    C3DVector* (__thiscall*)(C3DVector* pFirst, C3DVector* pLast, std::uint32_t filter, void* pScratch, int zero);
using CopyImplPlainFn =
    C3DVector* (__thiscall*)(C3DVector* pFirst, C3DVector* pLast, void* pDest, void* pScratch, int zero);
using PositionFilterNearbyThenFindFn =
    std::uint32_t (__cdecl*)(float nearbyDistance, float x, float y, float z);
using TeleportOutCtorFn =
    CCreatureActionBase* (__thiscall*)(void* pStorage, CThingCreatureBase* pCreature, const C3DVector* pDestination, bool flag);
using LightningOrbDtorFn =
    void (__thiscall*)(void* pStorage);
using RotateSeedFn =
    std::uint32_t (__cdecl*)(std::uint32_t value);

constexpr auto kCopyImpl = reinterpret_cast<CopyImplFilteredFn>(0x0074C940);
constexpr auto kCopyImplPlain = reinterpret_cast<CopyImplPlainFn>(0x0074C940);
constexpr auto kPositionFilterNearbyThenFind = reinterpret_cast<PositionFilterNearbyThenFindFn>(0x008ABFC0);
constexpr auto kTeleportOutCtor = reinterpret_cast<TeleportOutCtorFn>(0x008BCDF0);
constexpr auto kLightningOrbDtor = reinterpret_cast<LightningOrbDtorFn>(0x00630260);
constexpr auto kGFROR13 = reinterpret_cast<RotateSeedFn>(0x00498020);

constexpr ETCInterfaceType kCreatureNavigationInterfaceType = static_cast<ETCInterfaceType>(7);
constexpr std::uint8_t kTeleportFlag = 0x10;
}

bool __thiscall CGameScriptInterface::EntityWillTeleportToArea(
    const CScriptThing& entity,
    C3DVector teleportPosition,
    float nearbyDistance,
    float searchRadius) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    if (!entityOverlay->m_pVTable->IsAvailableForScript(&entity))
    {
        return false;
    }

    CVertexBufferWin32* const creatureThing = entityOverlay->m_pVTable->GetThing(&entity);
    if ((reinterpret_cast<const CThingCreatureBaseOverlay*>(creatureThing)->m_Flags6C & kTeleportFlag) == 0)
    {
        return false;
    }

    const auto* const selfOverlay = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
    auto* const worldOverlay = reinterpret_cast<CWorldOverlay*>(selfOverlay->m_pWorld);

    C3DVectorBuffer validPositions{};
    NCreatureHiding::GetValidPositions(
        &validPositions,
        worldOverlay->m_pVTable->GetWorldMap(selfOverlay->m_pWorld),
        worldOverlay->m_pThingSearchTools,
        &teleportPosition,
        searchRadius,
        false,
        nullptr);

    C3DVector* const oldEnd = validPositions.m_pEnd;
    std::uint8_t copyScratch = 0;
    const std::uint32_t filterResult = kPositionFilterNearbyThenFind(
        nearbyDistance,
        teleportPosition.x,
        teleportPosition.y,
        teleportPosition.z);
    validPositions.m_pEnd = kCopyImpl(
        oldEnd,
        validPositions.m_pEnd,
        filterResult,
        &copyScratch,
        0);

    std::uint32_t candidateCount =
        static_cast<std::uint32_t>(validPositions.m_pEnd - validPositions.m_pBegin);

    if (candidateCount != 0)
    {
        CTCCreatureNavigation* creatureNavigation;
        if (reinterpret_cast<const CThingCreatureBaseOverlay*>(creatureThing)->m_InterfaceFlags20 < 0)
        {
            auto* entry =
                reinterpret_cast<CThingCreatureBaseOverlay*>(creatureThing)
                    ->m_InterfaceMap44.LowerBound(&kCreatureNavigationInterfaceType);
            if ((entry == reinterpret_cast<CThingCreatureBaseOverlay*>(creatureThing)->m_InterfaceMap44.m_pEnd) ||
                (7 < static_cast<std::int32_t>(entry->m_Key)))
            {
                entry = reinterpret_cast<CThingCreatureBaseOverlay*>(creatureThing)->m_InterfaceMap44.m_pEnd;
            }
            creatureNavigation = reinterpret_cast<CTCCreatureNavigation*>(entry->m_Value);
        }

        do
        {
            std::uint32_t* const seed =
                reinterpret_cast<std::uint32_t*>(CVertexBufferWin32::DoSizeof(
                    *reinterpret_cast<CVertexBufferWin32* const*>(reinterpret_cast<const std::byte*>(this) + 0x04)));
            const std::uint32_t nextSeed = (*seed * 0x24A1u) + 0x24DFu;
            *seed = nextSeed;

            std::uint32_t randomValue = kGFROR13(nextSeed);
            *seed = randomValue;

            std::uint32_t randomIndex;
            if (candidateCount == 0)
            {
                randomIndex = 0;
            }
            else
            {
                randomIndex = randomValue % candidateCount;
            }

            C3DVector* const candidate =
                reinterpret_cast<C3DVector*>(reinterpret_cast<std::byte*>(validPositions.m_pBegin) + (randomIndex * 0x0C));
            C3DVector* const sourcePosition = CVertexBufferWin32::DoSizeof(creatureThing);
            if (CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(
                    creatureNavigation,
                    sourcePosition,
                    candidate,
                    0.0f))
            {
                C3DVector chosenPosition;
                chosenPosition.x = candidate->x;
                chosenPosition.y = candidate->y;
                chosenPosition.z = candidate->z;

                CCreatureAction_TeleportOut_Storage actionStorage;
                CCreatureActionBase* const action = kTeleportOutCtor(
                    &actionStorage,
                    reinterpret_cast<CThingCreatureBase*>(creatureThing),
                    &chosenPosition,
                    false);
                const bool result =
                    reinterpret_cast<CThingCreatureBase*>(creatureThing)->SetCurrentAction(action);
                kLightningOrbDtor(&actionStorage);

                if (validPositions.m_pBegin != nullptr)
                {
                    std::free(validPositions.m_pBegin);
                }
                return result;
            }

            void* const eraseAt =
                reinterpret_cast<std::byte*>(validPositions.m_pBegin) + (randomIndex * 0x0C);
            C3DVector* const next =
                reinterpret_cast<C3DVector*>(reinterpret_cast<std::byte*>(eraseAt) + 0x0C);
            if (next != validPositions.m_pEnd)
            {
                kCopyImplPlain(next, validPositions.m_pEnd, eraseAt, &copyScratch, 0);
            }

            validPositions.m_pEnd =
                reinterpret_cast<C3DVector*>(reinterpret_cast<std::byte*>(validPositions.m_pEnd) - 0x0C);
            candidateCount =
                static_cast<std::uint32_t>(validPositions.m_pEnd - validPositions.m_pBegin);
        } while (candidateCount != 0);
    }

    if (validPositions.m_pBegin != nullptr)
    {
        std::free(validPositions.m_pBegin);
    }

    return false;
}