#include <cstddef>
#include <cstdint>
#include <new>

class C3DVector;
class CGameScriptInterface;
class CScriptThing;
class CTCBase;
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

namespace NHeroInformationScreens
{
class CBase
{
public:
    __thiscall CBase();
};
} // namespace NHeroInformationScreens

class CGameScriptInterface
{
public:
    void __thiscall SetWanderCentrePoint(CScriptThing thing, C3DVector wanderCentre);
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

using SharedCleanupFn = void(__thiscall*)(void*);

struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CThing* (__thiscall* GetThing)(void* pThis); // 0x2C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);

struct CScriptThingObjectOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingObjectOverlay, m_pVTable) == 0x00);

struct CScriptThingSharedCleanupOverlay
{
    std::int32_t m_RefCount;   // 0x00
    SharedCleanupFn m_Destroy; // 0x04
    void* m_pDestroyThis;      // 0x08
};

static_assert(offsetof(CScriptThingSharedCleanupOverlay, m_RefCount) == 0x00);
static_assert(offsetof(CScriptThingSharedCleanupOverlay, m_Destroy) == 0x04);
static_assert(offsetof(CScriptThingSharedCleanupOverlay, m_pDestroyThis) == 0x08);
static_assert(sizeof(CScriptThingSharedCleanupOverlay) == 0x0C);

struct CScriptThingArgumentOverlay
{
    CScriptThingObjectOverlay* m_pThingObject;          // 0x00
    CScriptThingSharedCleanupOverlay* m_pSharedCleanup; // 0x04
};

static_assert(offsetof(CScriptThingArgumentOverlay, m_pThingObject) == 0x00);
static_assert(offsetof(CScriptThingArgumentOverlay, m_pSharedCleanup) == 0x04);
static_assert(sizeof(CScriptThingArgumentOverlay) == 0x08);

struct SetWanderCentrePointTargetOverlay
{
    std::byte m_Pad00[0x34];
    std::uint32_t m_Flags34; // 0x34
    std::byte m_Pad38[0x0C];
    TCInterfaceMap m_InterfaceMap44; // 0x44
};

static_assert(offsetof(SetWanderCentrePointTargetOverlay, m_Flags34) == 0x34);
static_assert(offsetof(SetWanderCentrePointTargetOverlay, m_InterfaceMap44) == 0x44);

struct SetWanderCentrePointReceiverOverlay
{
    std::byte m_Pad00[0x14];
    std::uint32_t m_Value14; // 0x14
    std::uint32_t m_Value18; // 0x18
    std::uint32_t m_Value1C; // 0x1C
};

static_assert(offsetof(SetWanderCentrePointReceiverOverlay, m_Value14) == 0x14);
static_assert(offsetof(SetWanderCentrePointReceiverOverlay, m_Value18) == 0x18);
static_assert(offsetof(SetWanderCentrePointReceiverOverlay, m_Value1C) == 0x1C);

struct C3DVectorValueOverlay
{
    std::uint32_t m_Value0; // 0x00
    std::uint32_t m_Value4; // 0x04
    std::uint32_t m_Value8; // 0x08
};

static_assert(offsetof(C3DVectorValueOverlay, m_Value0) == 0x00);
static_assert(offsetof(C3DVectorValueOverlay, m_Value4) == 0x04);
static_assert(offsetof(C3DVectorValueOverlay, m_Value8) == 0x08);
static_assert(sizeof(C3DVectorValueOverlay) == 0x0C);

struct DecompiledLocalOverlay
{
    void* m_Param1;                             // param_1
    CScriptThingObjectOverlay* m_Param2;        // param_2
    CScriptThingSharedCleanupOverlay* m_Param3; // param_3
};

static_assert(offsetof(DecompiledLocalOverlay, m_Param1) == 0x00);
static_assert(offsetof(DecompiledLocalOverlay, m_Param2) == 0x04);
static_assert(offsetof(DecompiledLocalOverlay, m_Param3) == 0x08);
static_assert(sizeof(DecompiledLocalOverlay) == 0x0C);

constexpr ETCInterfaceType kWanderInterfaceType = static_cast<ETCInterfaceType>(0xAA);
constexpr std::uint32_t kHasWanderInterfaceMask = 0x00000400;
constexpr void* kScalarDeletingDestructor_01238C8C = reinterpret_cast<void*>(0x01238C8C);
} // namespace

void __thiscall CGameScriptInterface::SetWanderCentrePoint(
    CScriptThing thing,
    C3DVector wanderCentre)
{
    const auto* const pThingArgument = reinterpret_cast<const CScriptThingArgumentOverlay*>(&thing);

    DecompiledLocalOverlay local;
    local.m_Param1 = this;
    local.m_Param2 = pThingArgument->m_pThingObject;
    local.m_Param3 = pThingArgument->m_pSharedCleanup;

    CThing* pThing;
    if ((local.m_Param2 == nullptr) ||
        ((pThing = local.m_Param2->m_pVTable->GetThing(local.m_Param2)) == nullptr))
    {
        local.m_Param1 = kScalarDeletingDestructor_01238C8C;
        if ((local.m_Param3 == nullptr) || (--local.m_Param3->m_RefCount != 0))
        {
            goto LAB_008A24A1;
        }

        local.m_Param3->m_Destroy(local.m_Param3->m_pDestroyThis);
    }
    else
    {
        if (local.m_Param2 == nullptr)
        {
            pThing = nullptr;
        }
        else
        {
            pThing = local.m_Param2->m_pVTable->GetThing(local.m_Param2);
        }

        auto* const pTarget = reinterpret_cast<SetWanderCentrePointTargetOverlay*>(pThing);
        if ((pTarget->m_Flags34 & kHasWanderInterfaceMask) != 0)
        {
            CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
                pTarget->m_InterfaceMap44.LowerBound(&kWanderInterfaceType);

            if ((pEntry == pTarget->m_InterfaceMap44.m_pEnd) ||
                (kWanderInterfaceType < pEntry->m_Key))
            {
                pEntry = pTarget->m_InterfaceMap44.m_pEnd;
            }

            local.m_Param1 = pEntry->m_Value;
        }

        const auto* const pWanderCentre =
            reinterpret_cast<const C3DVectorValueOverlay*>(&wanderCentre);
        auto* const pReceiver =
            reinterpret_cast<SetWanderCentrePointReceiverOverlay*>(local.m_Param1);

        pReceiver->m_Value14 = pWanderCentre->m_Value0;
        pReceiver->m_Value18 = pWanderCentre->m_Value4;
        pReceiver->m_Value1C = pWanderCentre->m_Value8;

        local.m_Param1 = kScalarDeletingDestructor_01238C8C;
        if ((local.m_Param3 == nullptr) || (--local.m_Param3->m_RefCount != 0))
        {
            goto LAB_008A24A1;
        }

        local.m_Param3->m_Destroy(local.m_Param3->m_pDestroyThis);
    }

    {
        CScriptThingSharedCleanupOverlay* const pShared = local.m_Param3;
        ::operator delete(pShared);
    }

LAB_008A24A1:
    local.m_Param3 = nullptr;
    local.m_Param2 = nullptr;
    ::new (static_cast<void*>(&local.m_Param1)) NHeroInformationScreens::CBase();
}