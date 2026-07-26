#include <cstddef>
#include <cstdint>

class CDefinitionManager;
class CEventBase;
class CExpressionDef;
class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCEntityEvents;
class CThingCreatureBase;

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

template <typename T>
struct CDefPointer
{
    T* m_pObject; // 0x00
};

static_assert(sizeof(CDefPointer<CExpressionDef const>) == 0x04);
static_assert(offsetof(CDefPointer<CExpressionDef const>, m_pObject) == 0x00);

class CCharString
{
public:
    CCharString(const char* pText, int length);
    ~CCharString();
};

class CDefinitionManager
{
public:
    template <typename T>
    void __thiscall GetDef(const CCharString* pName, CDefPointer<T const>* pDef);
};

class CTCEntityEvents
{
public:
    void __thiscall AddEvent(CEventBase* pEvent);
};

class CGameScriptInterface
{
public:
    void __thiscall EntityFollowThing(
        const CScriptThing& follower,
        const CScriptThing& target,
        float followDistance,
        bool unkFlag) const;
};

extern "C" CDefinitionManager* __cdecl GFGetPreMainMemoryUsed();
extern "C" void* __cdecl operator_new(unsigned int size);

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CScriptThing_VTable_Overlay
{
    std::byte m_Pad00[0x2C];
    CThingCreatureBase* (__thiscall* GetThing)(const CScriptThing* pScriptThing); // 0x2C
    std::byte m_Pad30[0xFC];
    bool (__thiscall* IsValidThing)(const CScriptThing* pScriptThing); // 0x12C
};

static_assert(offsetof(CScriptThing_VTable_Overlay, GetThing) == 0x2C);
static_assert(offsetof(CScriptThing_VTable_Overlay, IsValidThing) == 0x12C);

struct CScriptThing_Overlay
{
    const CScriptThing_VTable_Overlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThing_Overlay, m_pVTable) == 0x00);

struct CThingCreatureBase_EntityFollowThing_Overlay
{
    std::byte m_Pad00[0x28];
    std::uint8_t m_Flags28; // 0x28
    std::byte m_Pad29[0x1B];
    TCInterfaceMap m_InterfaceMap; // 0x44
};

static_assert(offsetof(CThingCreatureBase_EntityFollowThing_Overlay, m_Flags28) == 0x28);
static_assert(offsetof(CThingCreatureBase_EntityFollowThing_Overlay, m_InterfaceMap) == 0x44);

struct RefCountedDefOverlay
{
    void** m_pVTable;        // 0x00
    std::int32_t m_RefCount; // 0x04
};

static_assert(offsetof(RefCountedDefOverlay, m_pVTable) == 0x00);
static_assert(offsetof(RefCountedDefOverlay, m_RefCount) == 0x04);

struct CEventExpressionPerformedTo_Overlay
{
    std::byte m_Pad00[0x20];
};

static_assert(sizeof(CEventExpressionPerformedTo_Overlay) == 0x20);

using NotifyFollowStateFn =
    void(__thiscall*)(const CGameScriptInterface* pThis, const CScriptThing* pFollower, int enabled);

extern "C" CEventBase* __thiscall
NEntityEvents__CEventExpressionPerformedTo__CEventExpressionPerformedTo(
    CEventExpressionPerformedTo_Overlay* pThis,
    CThingCreatureBase* pTarget,
    int zero,
    const CDefPointer<CExpressionDef const>* pExpressionDef,
    long followDistance,
    std::uint32_t unkFlag);
}

void __thiscall CGameScriptInterface::EntityFollowThing(
    const CScriptThing& follower,
    const CScriptThing& target,
    float followDistance,
    bool unkFlag) const
{
    const auto* const followerOverlay = reinterpret_cast<const CScriptThing_Overlay*>(&follower);
    if (!followerOverlay->m_pVTable->IsValidThing(&follower))
    {
        return;
    }

    const auto* const targetOverlay = reinterpret_cast<const CScriptThing_Overlay*>(&target);
    if (!targetOverlay->m_pVTable->IsValidThing(&target))
    {
        return;
    }

    CThingCreatureBase* const pTargetThing = targetOverlay->m_pVTable->GetThing(&target);
    auto* const pFollowerThing = reinterpret_cast<CThingCreatureBase_EntityFollowThing_Overlay*>(
        followerOverlay->m_pVTable->GetThing(&follower));

    CDefPointer<CExpressionDef const> expressionDef{};
    {
        CCharString expressionName("EXPRESSION_FOLLOW", -1);
        CDefinitionManager* const pDefinitionManager = GFGetPreMainMemoryUsed();
        pDefinitionManager->GetDef<CExpressionDef>(&expressionName, &expressionDef);
    }

    if ((pFollowerThing->m_Flags28 & 0x04) != 0)
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x42);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            pFollowerThing->m_InterfaceMap.LowerBound(&interfaceType);

        if ((pEntry == pFollowerThing->m_InterfaceMap.m_pEnd) ||
            (0x42 < static_cast<std::int32_t>(pEntry->m_Key)))
        {
            pEntry = pFollowerThing->m_InterfaceMap.m_pEnd;
        }

        auto* const pEntityEvents = reinterpret_cast<CTCEntityEvents*>(pEntry->m_Value);

        const auto notifyFollowState = reinterpret_cast<NotifyFollowStateFn>(
            (*reinterpret_cast<void* const* const*>(this))[0x3AC / sizeof(void*)]);
        notifyFollowState(this, &follower, 1);

        CEventBase* pEvent = nullptr;
        auto* const pStorage =
            static_cast<CEventExpressionPerformedTo_Overlay*>(operator_new(0x20));
        if (pStorage == nullptr)
        {
            pEvent = nullptr;
        }
        else
        {
            const std::uint32_t uVar8 = static_cast<std::uint32_t>(unkFlag);
            const long lVar6 = static_cast<long>(followDistance);
            pEvent = NEntityEvents__CEventExpressionPerformedTo__CEventExpressionPerformedTo(
                pStorage,
                pTargetThing,
                0,
                &expressionDef,
                lVar6,
                uVar8);
        }

        pEntityEvents->AddEvent(pEvent);
    }

    if (expressionDef.m_pObject != nullptr)
    {
        auto* const pDefOverlay =
            reinterpret_cast<RefCountedDefOverlay*>(const_cast<CExpressionDef*>(expressionDef.m_pObject));
        --pDefOverlay->m_RefCount;
        if (pDefOverlay->m_RefCount == 0)
        {
            reinterpret_cast<void(__thiscall*)(void*)>(pDefOverlay->m_pVTable[1])(pDefOverlay);
        }
    }
}