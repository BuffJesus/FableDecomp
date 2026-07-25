#include <cstddef>
#include <cstdint>
#include <new>

class CBaseObject;
class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCCarrying;

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

class CBaseIntelligentPointer
{
public:
    CBaseIntelligentPointer();
    ~CBaseIntelligentPointer();

    void __thiscall SetPItem(CBaseObject* pItem);
};

class CTCCarrying
{
public:
    void __thiscall SetAllCarriedThingsAsBypassFilter(void* pScreenFilterContext);
};

class CGameScriptInterface
{
public:
    void __thiscall SetThingAndCarriedItemsNotAffectedByScreenFilter(
        CScriptThing& thing,
        void* pScreenFilterContext) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CBaseObject* (__thiscall* m_GetThing)(CScriptThing* pThis); // 0x2C
};

static_assert(offsetof(CScriptThingVTableOverlay, m_GetThing) == 0x2C);

struct CScriptThingOverlay
{
    CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CGameScriptInterfaceSelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};

static_assert(offsetof(CGameScriptInterfaceSelfOverlay, m_Field04) == 0x04);

struct CGameScriptInterfaceField04Overlay
{
    std::byte m_Pad00[0x18];
    void* m_Field18; // 0x18
};

static_assert(offsetof(CGameScriptInterfaceField04Overlay, m_Field18) == 0x18);

struct CGameScriptInterfaceField18Overlay
{
    std::byte m_Pad00[0x1964];
    void* m_Field1964; // 0x1964
};

static_assert(offsetof(CGameScriptInterfaceField18Overlay, m_Field1964) == 0x1964);

struct CThingOverlay
{
    std::byte m_Pad00[0x28];
    std::uint32_t m_Flags28; // 0x28
    std::byte m_Pad2C[0x18];
    TCInterfaceMap m_InterfaceMap44; // 0x44
};

static_assert(offsetof(CThingOverlay, m_Flags28) == 0x28);
static_assert(offsetof(CThingOverlay, m_InterfaceMap44) == 0x44);

struct CBaseIntelligentPointerVPtrOverlay
{
    void* m_pVTable; // 0x00
};

static_assert(offsetof(CBaseIntelligentPointerVPtrOverlay, m_pVTable) == 0x00);

#pragma pack(push, 1)
struct ScreenFilterBypassThingEntry
{
    CBaseIntelligentPointer m_ThingPointer; // 0x00
    std::uint8_t m_BypassFlag;              // 0x08
};
#pragma pack(pop)

static_assert(offsetof(ScreenFilterBypassThingEntry, m_ThingPointer) == 0x00);
static_assert(offsetof(ScreenFilterBypassThingEntry, m_BypassFlag) == 0x08);
static_assert(sizeof(ScreenFilterBypassThingEntry) == 0x09);

struct ScreenFilterBypassThingVector
{
    ScreenFilterBypassThingEntry* m_pBegin;       // 0x00
    ScreenFilterBypassThingEntry* m_pEnd;         // 0x04
    ScreenFilterBypassThingEntry* m_pCapacityEnd; // 0x08

    void __thiscall PushBack(const ScreenFilterBypassThingEntry* pEntry);
};

static_assert(offsetof(ScreenFilterBypassThingVector, m_pBegin) == 0x00);
static_assert(offsetof(ScreenFilterBypassThingVector, m_pEnd) == 0x04);
static_assert(offsetof(ScreenFilterBypassThingVector, m_pCapacityEnd) == 0x08);
static_assert(sizeof(ScreenFilterBypassThingVector) == 0x0C);

using RawField1964HelperFn =
    void(__thiscall*)(void* pThis, const ScreenFilterBypassThingVector* pEntries, void* pContext);

inline constexpr RawField1964HelperFn RawField1964Helper =
    reinterpret_cast<RawField1964HelperFn>(0x006B83CD);

extern "C" void __cdecl free(void* pMemory);
extern void PTR__vector_deleting_destructor__01238c6c;
} // namespace

void __thiscall CGameScriptInterface::SetThingAndCarriedItemsNotAffectedByScreenFilter(
    CScriptThing& thing,
    void* pScreenFilterContext) const
{
    auto* const thingOverlay = reinterpret_cast<CScriptThingOverlay*>(&thing);
    CBaseObject* const pThing = thingOverlay->m_pVTable->m_GetThing(&thing);

    ScreenFilterBypassThingVector bypassThings{};
    alignas(ScreenFilterBypassThingEntry) std::byte tempEntryStorage[sizeof(ScreenFilterBypassThingEntry)];
    auto* const pTempEntry = reinterpret_cast<ScreenFilterBypassThingEntry*>(tempEntryStorage);

    new (&pTempEntry->m_ThingPointer) CBaseIntelligentPointer();
    reinterpret_cast<CBaseIntelligentPointerVPtrOverlay*>(&pTempEntry->m_ThingPointer)->m_pVTable =
        reinterpret_cast<void*>(&PTR__vector_deleting_destructor__01238c6c);
    pTempEntry->m_ThingPointer.SetPItem(pThing);
    pTempEntry->m_BypassFlag = 1;
    bypassThings.PushBack(pTempEntry);
    pTempEntry->m_ThingPointer.~CBaseIntelligentPointer();

    const auto* const selfOverlay = reinterpret_cast<const CGameScriptInterfaceSelfOverlay*>(this);
    auto* const field04 = reinterpret_cast<CGameScriptInterfaceField04Overlay*>(selfOverlay->m_Field04);
    auto* const field18 = reinterpret_cast<CGameScriptInterfaceField18Overlay*>(field04->m_Field18);

    RawField1964Helper(field18->m_Field1964, &bypassThings, pScreenFilterContext);

    auto* const objectOverlay = reinterpret_cast<CThingOverlay*>(pThing);
    if ((objectOverlay->m_Flags28 & 0x40u) != 0)
    {
        ETCInterfaceType carryingInterfaceType = static_cast<ETCInterfaceType>(0x46);
        CKeyPair<ETCInterfaceType, CTCBase*>* pCarryingEntry =
            objectOverlay->m_InterfaceMap44.LowerBound(&carryingInterfaceType);

        if ((pCarryingEntry == objectOverlay->m_InterfaceMap44.m_pEnd) ||
            (carryingInterfaceType < pCarryingEntry->m_Key))
        {
            pCarryingEntry = objectOverlay->m_InterfaceMap44.m_pEnd;
        }

        reinterpret_cast<CTCCarrying*>(pCarryingEntry->m_Value)
            ->SetAllCarriedThingsAsBypassFilter(pScreenFilterContext);
    }

    ScreenFilterBypassThingEntry* pCurrent = bypassThings.m_pBegin;
    while (pCurrent != bypassThings.m_pEnd)
    {
        pCurrent->m_ThingPointer.~CBaseIntelligentPointer();
        ++pCurrent;
    }

    if (bypassThings.m_pBegin != nullptr)
    {
        free(bypassThings.m_pBegin);
    }
}