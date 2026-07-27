#include <cstddef>
#include <cstdint>

class C3DVector;
class CCharString;
class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCCreatureNavigation;
class CThing;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;     // 0x00
    TValue m_Value; // 0x04
};

static_assert(offsetof(CKeyPair<std::int32_t, CTCBase*>, m_Key) == 0x00);
static_assert(offsetof(CKeyPair<std::int32_t, CTCBase*>, m_Value) == 0x04);
static_assert(sizeof(CKeyPair<std::int32_t, CTCBase*>) == 0x08);

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

static_assert(
    offsetof(CVectorMap<std::int32_t, CTCBase*, CKeyPairCompareLess<std::int32_t, CTCBase*>>, m_pEnd) ==
    0x04);

namespace NHeroInformationScreens
{
class CBase
{
public:
    __thiscall CBase();
};
}

class CTCCreatureNavigation
{
public:
    bool __thiscall GetNearbyWanderPosition(
        const C3DVector* pOrigin,
        float radius,
        C3DVector* pResult,
        bool allowBlockedPositions);
};

class CGameScriptInterface
{
public:
    CScriptThing* __thiscall CreateCreatureNearby(
        CScriptThing* pResult,
        const CCharString& resRef,
        const C3DVector& position,
        float radius,
        const CCharString& scriptName,
        bool someFlag) const;
};

namespace
{
using ETCInterfaceTypeRaw = std::int32_t;
constexpr ETCInterfaceTypeRaw kCreatureNavigationInterfaceType = 7;

struct CScriptThingWrapperOverlay
{
    void* m_pWrapperVftable; // 0x00
    void* m_Field04;         // 0x04
    std::int32_t* m_pShared; // 0x08
};

static_assert(offsetof(CScriptThingWrapperOverlay, m_pWrapperVftable) == 0x00);
static_assert(offsetof(CScriptThingWrapperOverlay, m_Field04) == 0x04);
static_assert(offsetof(CScriptThingWrapperOverlay, m_pShared) == 0x08);
static_assert(sizeof(CScriptThingWrapperOverlay) == 0x0C);

struct CScriptThingHandleVftableOverlay
{
    std::byte m_Pad00[0x2C];
    CThing* (__thiscall* GetThing)(void* pThis); // 0x2C
    std::byte m_Pad30[0x12C - 0x30];
    bool (__thiscall* IsAvailableForScript)(void* pThis); // 0x12C
};

static_assert(offsetof(CScriptThingHandleVftableOverlay, GetThing) == 0x2C);
static_assert(offsetof(CScriptThingHandleVftableOverlay, IsAvailableForScript) == 0x12C);

struct CScriptThingHandleOverlay
{
    CScriptThingHandleVftableOverlay* m_pVftable; // 0x00
};

static_assert(offsetof(CScriptThingHandleOverlay, m_pVftable) == 0x00);

using TCInterfaceMap =
    CVectorMap<ETCInterfaceTypeRaw, CTCBase*, CKeyPairCompareLess<ETCInterfaceTypeRaw, CTCBase*>>;

struct CThingMoveVftableOverlay
{
    std::byte m_Pad00[0x7C];
    void (__thiscall* SetPosition)(void* pThis, const C3DVector* pPosition); // 0x7C
};

static_assert(offsetof(CThingMoveVftableOverlay, SetPosition) == 0x7C);

struct CThingMoveOverlay
{
    CThingMoveVftableOverlay* m_pVftable; // 0x00
};

struct CThingCreateCreatureNearbyOverlay
{
    std::byte m_Pad00[0x20];
    std::int8_t m_InterfaceFlags20; // 0x20
    std::byte m_Pad21[0x44 - 0x21];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x60 - 0x4C];
    CThingMoveOverlay* m_Field60; // 0x60
};

static_assert(offsetof(CThingCreateCreatureNearbyOverlay, m_InterfaceFlags20) == 0x20);
static_assert(offsetof(CThingCreateCreatureNearbyOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(CThingCreateCreatureNearbyOverlay, m_Field60) == 0x60);

struct CGameScriptInterfaceVftableOverlay
{
    std::byte m_Pad00[0x16C];
    void (__thiscall* CreateCreature)(
        const CGameScriptInterface* pThis,
        CScriptThingWrapperOverlay* pResult,
        const CCharString& resRef,
        const C3DVector& position,
        const CCharString& scriptName,
        bool someFlag); // 0x16C
};

static_assert(offsetof(CGameScriptInterfaceVftableOverlay, CreateCreature) == 0x16C);

struct CGameScriptInterfaceOverlay
{
    CGameScriptInterfaceVftableOverlay* m_pVftable; // 0x00
    void* m_Field04;                                // 0x04
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_Field04) == 0x04);

using GetRandomContextFn = void* (__thiscall*)(void* pThis);
using GetRandomFloatFn = float (__thiscall*)(void* pThis, float maxValue);
using GetThingPositionFn = const C3DVector* (__thiscall*)(CThing* pThis);
using SharedReleaseFn = void (*)();

constexpr auto kGetRandomContext = reinterpret_cast<GetRandomContextFn>(0x0049D821);
constexpr auto kGetRandomFloat = reinterpret_cast<GetRandomFloatFn>(0x004978C6);
constexpr auto kGetThingPosition = reinterpret_cast<GetThingPositionFn>(0x004C73EC);

constexpr auto kScriptThingScalarDeletingDtor = reinterpret_cast<void*>(0x01238C8C);
constexpr auto kMinimumNearbyRadius = reinterpret_cast<const float*>(0x01254DA8);

inline CThing* GetThingFromHandle(void* pHandleObject)
{
    return reinterpret_cast<CScriptThingHandleOverlay*>(pHandleObject)->m_pVftable->GetThing(pHandleObject);
}

inline bool IsHandleAvailableForScript(void* pHandleObject)
{
    return reinterpret_cast<CScriptThingHandleOverlay*>(pHandleObject)->m_pVftable->IsAvailableForScript(
        pHandleObject);
}
} // namespace

CScriptThing* __thiscall CGameScriptInterface::CreateCreatureNearby(
    CScriptThing* pResult,
    const CCharString& resRef,
    const C3DVector& position,
    float radius,
    const CCharString& scriptName,
    bool someFlag) const
{
    CScriptThingWrapperOverlay localResult;
    reinterpret_cast<const CGameScriptInterfaceOverlay*>(this)->m_pVftable->CreateCreature(
        this,
        &localResult,
        resRef,
        position,
        scriptName,
        someFlag);

    void* handleObject = localResult.m_Field04;
    if ((handleObject != nullptr) && IsHandleAvailableForScript(handleObject))
    {
        CThing* const pThing = GetThingFromHandle(handleObject);
        auto* const pThingOverlay = reinterpret_cast<CThingCreateCreatureNearbyOverlay*>(pThing);

        if (pThingOverlay->m_InterfaceFlags20 < 0)
        {
            ETCInterfaceTypeRaw interfaceType = kCreatureNavigationInterfaceType;
            auto* pEntry = pThingOverlay->m_InterfaceMap44.LowerBound(&interfaceType);
            if ((pEntry == pThingOverlay->m_InterfaceMap44.m_pEnd) ||
                (kCreatureNavigationInterfaceType < pEntry->m_Key))
            {
                pEntry = pThingOverlay->m_InterfaceMap44.m_pEnd;
            }

            auto* const pNavigation = reinterpret_cast<CTCCreatureNavigation*>(pEntry->m_Value);

            float nearbyRadius = 0.8f;
            if (*kMinimumNearbyRadius < radius)
            {
                const float randomSpan = radius - *kMinimumNearbyRadius;
                void* const pRandomContext =
                    kGetRandomContext(reinterpret_cast<const CGameScriptInterfaceOverlay*>(this)->m_Field04);
                nearbyRadius = kGetRandomFloat(pRandomContext, randomSpan);
                nearbyRadius = nearbyRadius + *kMinimumNearbyRadius;
                handleObject = localResult.m_Field04;
            }

            CThing* pOriginThing;
            if (handleObject == nullptr)
            {
                pOriginThing = nullptr;
            }
            else
            {
                pOriginThing = GetThingFromHandle(handleObject);
            }

            C3DVector nearbyPosition;
            const bool foundPosition = pNavigation->GetNearbyWanderPosition(
                kGetThingPosition(pOriginThing),
                nearbyRadius,
                &nearbyPosition,
                false);

            if (foundPosition)
            {
                void* moveHandleObject = localResult.m_Field04;
                CThing* pMoveThing;
                if (moveHandleObject == nullptr)
                {
                    pMoveThing = nullptr;
                }
                else
                {
                    pMoveThing = GetThingFromHandle(moveHandleObject);
                }

                auto* const pMoveThingOverlay = reinterpret_cast<CThingCreateCreatureNearbyOverlay*>(pMoveThing);
                pMoveThingOverlay->m_Field60->m_pVftable->SetPosition(pMoveThingOverlay->m_Field60, &nearbyPosition);
            }
        }
    }

    auto* const pOut = reinterpret_cast<CScriptThingWrapperOverlay*>(pResult);
    pOut->m_pWrapperVftable = kScriptThingScalarDeletingDtor;
    pOut->m_Field04 = localResult.m_Field04;
    pOut->m_pShared = localResult.m_pShared;
    if (localResult.m_pShared != nullptr)
    {
        ++(*localResult.m_pShared);
    }

    localResult.m_pWrapperVftable = kScriptThingScalarDeletingDtor;
    if ((localResult.m_pShared != nullptr) && (--(*localResult.m_pShared) == 0))
    {
        reinterpret_cast<SharedReleaseFn>(localResult.m_pShared[1])();
        operator delete(localResult.m_pShared);
    }

    localResult.m_Field04 = nullptr;
    localResult.m_pShared = nullptr;
    NHeroInformationScreens::CBase::CBase(reinterpret_cast<NHeroInformationScreens::CBase*>(&localResult));
    return pResult;
}