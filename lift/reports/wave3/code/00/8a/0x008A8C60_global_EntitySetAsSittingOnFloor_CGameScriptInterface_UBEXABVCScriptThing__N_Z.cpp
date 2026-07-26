#include <cstddef>
#include <cstdint>

class CAnimFilterBase;
class CAnimationEntry;
class CCharString;
class CCreatureActionBase;
class CDefString;
class CFileInfo;
class CGameScriptInterface;
class CScriptThing;
class CThingCreatureBase;

enum ETCInterfaceType : std::int32_t
{
};

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;     // 0x00
    TValue m_Value; // 0x04
};

static_assert(offsetof(CKeyPair<ETCInterfaceType, void*>, m_Key) == 0x00);
static_assert(offsetof(CKeyPair<ETCInterfaceType, void*>, m_Value) == 0x04);
static_assert(sizeof(CKeyPair<ETCInterfaceType, void*>) == 0x08);

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey) const;
};

class CCharString
{
public:
    CCharString(const char* pText, int length);
    ~CCharString();
};

class CDefString
{
public:
    operator CCharString() const;
};

class CThingCreatureBase
{
public:
    void __thiscall FinishCurrentAction();
    CAnimationEntry* __thiscall GetRandomAnimFromNameWithFilter(
        const CCharString& name,
        CAnimFilterBase* pFilter);
    void __thiscall SetCurrentAction(CCreatureActionBase* pAction);
};

class CGameScriptInterface
{
public:
    void __thiscall EntitySetAsSittingOnFloor(
        const CScriptThing& entity,
        bool sittingOnFloor) const;
};

namespace
{
template <typename TKey, typename TValue>
class CKeyPairCompareLess;

using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, void*, CKeyPairCompareLess<ETCInterfaceType, void*>>;

struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CThingCreatureBase* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
    std::byte m_Pad30[0xFC];
    bool (__thiscall* IsValidThing)(const CScriptThing* pThis); // 0x12C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);
static_assert(offsetof(CScriptThingVTableOverlay, IsValidThing) == 0x12C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct TCInterfaceMapOverlay
{
    CKeyPair<ETCInterfaceType, void*>* m_pBegin; // 0x00
    CKeyPair<ETCInterfaceType, void*>* m_pEnd;   // 0x04
};

static_assert(offsetof(TCInterfaceMapOverlay, m_pBegin) == 0x00);
static_assert(offsetof(TCInterfaceMapOverlay, m_pEnd) == 0x04);
static_assert(sizeof(TCInterfaceMapOverlay) == 0x08);

struct CThingCreatureBaseOverlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24; // 0x24
    std::byte m_Pad28[0x1C];
    TCInterfaceMapOverlay m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x20];
    std::uint8_t m_Byte6C; // 0x6C
};

static_assert(offsetof(CThingCreatureBaseOverlay, m_Flags24) == 0x24);
static_assert(offsetof(CThingCreatureBaseOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(CThingCreatureBaseOverlay, m_Byte6C) == 0x6C);

struct CAnimationEntryOverlay
{
    std::byte m_Pad00[0x08];
    CDefString m_Name; // 0x08
};

static_assert(offsetof(CAnimationEntryOverlay, m_Name) == 0x08);

struct CStackAnimFilterOverlay
{
    void* m_pVTable; // 0x00
};

static_assert(offsetof(CStackAnimFilterOverlay, m_pVTable) == 0x00);
static_assert(sizeof(CStackAnimFilterOverlay) == 0x04);

struct CCountedPointerFileInfoConstOverlay
{
    const CFileInfo* m_pObject; // 0x00
    std::int32_t* m_pRefCount;  // 0x04
};

static_assert(offsetof(CCountedPointerFileInfoConstOverlay, m_pObject) == 0x00);
static_assert(offsetof(CCountedPointerFileInfoConstOverlay, m_pRefCount) == 0x04);
static_assert(sizeof(CCountedPointerFileInfoConstOverlay) == 0x08);

struct CCreatureActionStorage
{
    std::byte m_Bytes[0x11C];
};

static_assert(sizeof(CCreatureActionStorage) == 0x11C);

extern "C" void* __cdecl operator_new(unsigned int size);

extern "C" void* __thiscall CountReferences_Interface31(void* pThis);

extern "C" CCreatureActionBase* __thiscall
CCreatureAction_GetUpFromSitting__CCreatureAction_GetUpFromSitting(
    void* pThis,
    CThingCreatureBase* pCreature,
    const CCharString* pActionName,
    int actionKind,
    bool unkFlag);

extern "C" CFileInfo* __thiscall ConstructSitDownFloorData(
    void* pThis,
    CThingCreatureBase* pCreature,
    const CCharString* pAnimationName);

extern "C" void __thiscall
CCountedPointerFileInfoConst__Ctor(
    CCountedPointerFileInfoConstOverlay* pThis,
    const CFileInfo* pObject);

extern "C" void __thiscall
CCountedPointerFileInfoConst__Dtor(
    CCountedPointerFileInfoConstOverlay* pThis);

extern "C" CCreatureActionBase* __thiscall
CCreatureAction_SitDown__CCreatureAction_SitDown(
    void* pThis,
    CThingCreatureBase* pCreature,
    CAnimationEntry* pAnimation,
    const CFileInfo* pObject,
    std::int32_t* pRefCount);

extern "C" void __thiscall DestroyGetUpFromSittingActionStorage(void* pThis);
extern "C" void __thiscall DestroySitDownActionStorage(void* pThis);

using GetActionKindFn = int(__thiscall*)(void* pThis);

constexpr ETCInterfaceType kInterfaceType31 = static_cast<ETCInterfaceType>(0x31);
constexpr std::uint32_t kHasInterface31Flag = 0x00020000;
constexpr std::uint8_t kCreatureFlag08 = 0x08;
constexpr int kSittingOnFloorActionKind = 10;

void* const g_SitOnFloorFilterState0 = reinterpret_cast<void*>(0x0127261C);
void* const g_SitOnFloorFilterState1 = reinterpret_cast<void*>(0x0122FFCC);
} // namespace

void __thiscall CGameScriptInterface::EntitySetAsSittingOnFloor(
    const CScriptThing& entity,
    bool sittingOnFloor) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    if (!entityOverlay->m_pVTable->IsValidThing(&entity))
    {
        return;
    }

    const CThingCreatureBase* const pThingForFlagTest =
        entityOverlay->m_pVTable->GetThing(&entity);
    const auto* const flagTestOverlay =
        reinterpret_cast<const CThingCreatureBaseOverlay*>(pThingForFlagTest);
    if ((flagTestOverlay->m_Byte6C & kCreatureFlag08) == 0)
    {
        return;
    }

    CThingCreatureBase* const pCreature = entityOverlay->m_pVTable->GetThing(&entity);
    const auto* const creatureOverlay =
        reinterpret_cast<const CThingCreatureBaseOverlay*>(pCreature);

    void* pInterface31 = nullptr;
    if ((creatureOverlay->m_Flags24 & kHasInterface31Flag) != 0)
    {
        auto* pEntry = reinterpret_cast<const TCInterfaceMap*>(&creatureOverlay->m_InterfaceMap44)
                           ->LowerBound(&kInterfaceType31);
        if ((pEntry == creatureOverlay->m_InterfaceMap44.m_pEnd) ||
            (static_cast<std::int32_t>(kInterfaceType31) <
             static_cast<std::int32_t>(pEntry->m_Key)))
        {
            pEntry = creatureOverlay->m_InterfaceMap44.m_pEnd;
        }
        pInterface31 = pEntry->m_Value;
    }

    if (!sittingOnFloor)
    {
        void* const pReferencedInterface = CountReferences_Interface31(pInterface31);
        const int actionKind =
            (reinterpret_cast<GetActionKindFn>((*reinterpret_cast<void***>(pReferencedInterface))[1]))(
                pReferencedInterface);

        if (actionKind == kSittingOnFloorActionKind)
        {
            pCreature->FinishCurrentAction();

            CCharString actionName("ST_STAND_UP_FROM_FLOOR", -1);
            CCreatureActionStorage actionStorage{};
            CCreatureActionBase* const pAction =
                CCreatureAction_GetUpFromSitting__CCreatureAction_GetUpFromSitting(
                    &actionStorage,
                    pCreature,
                    &actionName,
                    kSittingOnFloorActionKind,
                    false);
            pCreature->SetCurrentAction(pAction);
            DestroyGetUpFromSittingActionStorage(&actionStorage);
        }

        return;
    }

    void* const pReferencedInterface = CountReferences_Interface31(pInterface31);
    const int actionKind =
        (reinterpret_cast<GetActionKindFn>((*reinterpret_cast<void***>(pReferencedInterface))[1]))(
            pReferencedInterface);

    if (actionKind != kSittingOnFloorActionKind)
    {
        pCreature->FinishCurrentAction();

        CAnimationEntry* pAnimation = nullptr;
        {
            CCharString sitOnFloorName("ST_SIT_ON_FLOOR", -1);
            CStackAnimFilterOverlay filterState{};
            filterState.m_pVTable = g_SitOnFloorFilterState0;
            pAnimation = pCreature->GetRandomAnimFromNameWithFilter(
                sitOnFloorName,
                reinterpret_cast<CAnimFilterBase*>(&filterState));
            filterState.m_pVTable = g_SitOnFloorFilterState1;
        }

        void* const pAuxStorage = operator_new(0x38);
        CFileInfo* pAuxData = nullptr;
        CCountedPointerFileInfoConstOverlay countedAuxData{};

        if (pAuxStorage != nullptr)
        {
            const auto* const animationOverlay =
                reinterpret_cast<const CAnimationEntryOverlay*>(pAnimation);
            CCharString animationName = animationOverlay->m_Name;
            pAuxData = ConstructSitDownFloorData(pAuxStorage, pCreature, &animationName);
            CCountedPointerFileInfoConst__Ctor(&countedAuxData, pAuxData);
        }
        else
        {
            CCountedPointerFileInfoConst__Ctor(&countedAuxData, nullptr);
        }

        if (countedAuxData.m_pRefCount != nullptr)
        {
            ++*countedAuxData.m_pRefCount;
        }

        CCreatureActionStorage actionStorage{};
        CCreatureActionBase* const pAction =
            CCreatureAction_SitDown__CCreatureAction_SitDown(
                &actionStorage,
                pCreature,
                pAnimation,
                countedAuxData.m_pObject,
                countedAuxData.m_pRefCount);
        pCreature->SetCurrentAction(pAction);
        DestroySitDownActionStorage(&actionStorage);
        CCountedPointerFileInfoConst__Dtor(&countedAuxData);
    }
}