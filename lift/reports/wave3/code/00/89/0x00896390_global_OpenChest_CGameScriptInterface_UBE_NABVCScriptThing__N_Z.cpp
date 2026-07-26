#include <cstddef>
#include <cstdint>
#include <new>
#include <utility>

class C3DVector;
class CBaseObject;
class CCreatureActionBase;
class CGameScriptInterface;
class CParticleMorphs
{
public:
    class CEntry;
};
class CScriptThing;
class CTCBase;
class CTCChest;
class CTCHero;
class CTCHeroOpinionDeedLog;
class CTCSinglePersonOccupiable;
class CThing;
class CThingAICreature;
class CThingBuilding;
class CThingCreatureBase;
class CThingPhysical;
class CVertexBufferWin32;
class CFaction;

enum class EHeroMorphType : std::int32_t;
enum ETCInterfaceType : std::int32_t
{
};
enum EOccupyType : std::int32_t
{
};
enum ETutorialCategory : std::int32_t
{
};
enum EOpinionDeedType : std::int32_t
{
};
enum EOpinionPostDeedType : std::int32_t
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
    ~CBaseIntelligentPointer();

    static CBaseObject* __thiscall GetPItem(CBaseIntelligentPointer* pThis);
};

template <typename T>
class CIntelligentPointer;

template <typename T>
bool IS_THING_ALIVE(T* pThing);

class CTCChest
{
public:
    void __thiscall Open();
};

class CTCHero
{
public:
    bool __thiscall GiveTutorialForCategory(ETutorialCategory category, bool forceDisplay);
};

class CTCSinglePersonOccupiable
{
public:
    bool __thiscall IsOfType(EOccupyType* pType);
};

class CThingCreatureBase
{
public:
    void __thiscall FinishCurrentAction();
    bool __thiscall SetCurrentAction(CCreatureActionBase* pAction);
};

class CTCHeroOpinionDeedLog
{
public:
    long __thiscall PostDeed(
        EOpinionDeedType deedType,
        const C3DVector* pPosition,
        CThing* pThing,
        CThingAICreature* pCreature,
        EOpinionPostDeedType postDeedType,
        bool arg6,
        bool arg7,
        CFaction* pFaction,
        bool arg9,
        long arg10);
};

class CVertexBufferWin32
{
public:
    static C3DVector* __thiscall DoSizeof(CVertexBufferWin32* pThis);
};

class CGameScriptInterface
{
public:
    bool __thiscall OpenChest(const CScriptThing& thing, bool useHero) const;
};

class CCreatureAction_GetOutOfBed
{
public:
    CCreatureAction_GetOutOfBed(
        CThingCreatureBase* pCreature,
        CThing* pTargetThing,
        bool occupyTypeFlag);
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CThing* (__thiscall* m_GetThing)(const CScriptThing* pThis); // 0x2C
    std::byte m_Pad30[0xFC];
    bool (__thiscall* m_RawSlot12C)(const CScriptThing* pThis); // 0x12C
};

static_assert(offsetof(CScriptThingVTableOverlay, m_GetThing) == 0x2C);
static_assert(offsetof(CScriptThingVTableOverlay, m_RawSlot12C) == 0x12C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CGameScriptInterfaceOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_Field14) == 0x14);

struct CThingOverlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24; // 0x24
    std::uint32_t m_Flags28; // 0x28
    std::byte m_Pad2C[0x08];
    std::uint32_t m_Flags34; // 0x34
    std::uint32_t m_Flags38; // 0x38
    std::byte m_Pad3C[0x08];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThingOverlay, m_Flags24) == 0x24);
static_assert(offsetof(CThingOverlay, m_Flags28) == 0x28);
static_assert(offsetof(CThingOverlay, m_Flags34) == 0x34);
static_assert(offsetof(CThingOverlay, m_Flags38) == 0x38);
static_assert(offsetof(CThingOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(CThingOverlay, m_Flags91) == 0x91);

struct CInterface33Overlay
{
    std::byte m_Pad00[0x0D];
    CBaseIntelligentPointer m_Pointer0D; // 0x0D
};

static_assert(offsetof(CInterface33Overlay, m_Pointer0D) == 0x0D);

struct CInterfaceD5Overlay
{
    std::byte m_Pad00[0x11];
    std::uint8_t m_Field11; // 0x11
};

static_assert(offsetof(CInterfaceD5Overlay, m_Field11) == 0x11);

struct CCreatureAction_GetOutOfBed_Storage
{
    std::byte m_Bytes[0xA8];
};

static_assert(sizeof(CCreatureAction_GetOutOfBed_Storage) == 0xA8);

struct CBaseIntelligentPointer_Storage
{
    std::byte m_Bytes[0x04];
};

static_assert(sizeof(CBaseIntelligentPointer_Storage) == 0x04);

struct RawStackSlot
{
    std::byte m_Bytes[0x04];
};

static_assert(sizeof(RawStackSlot) == 0x04);

struct RawPhysicsMeshInfoStorage
{
    std::byte m_Bytes[0x08];
};

static_assert(sizeof(RawPhysicsMeshInfoStorage) == 0x08);

struct RegisterPair
{
    void* m_Eax;
    void* m_Edx;
};

extern void* DAT_013B8790;

using Catch_0040D959_t = std::uint64_t(__thiscall*)(void*);
using CPhysicsMeshInfo___CPhysicsMeshInfo_t = std::uint64_t(__thiscall*)(void*);

inline constexpr auto Catch_0040D959 = reinterpret_cast<Catch_0040D959_t>(0x0040D959);
inline constexpr auto CPhysicsMeshInfo___CPhysicsMeshInfo =
    reinterpret_cast<CPhysicsMeshInfo___CPhysicsMeshInfo_t>(0x00575450);
inline constexpr std::uintptr_t kConsValAddress = 0x00488DC2;

constexpr ETCInterfaceType kChestInterfaceType = static_cast<ETCInterfaceType>(0x3C);
constexpr ETCInterfaceType kHeroInterfaceType = static_cast<ETCInterfaceType>(0x29);
constexpr ETCInterfaceType kOwnerInterfaceType = static_cast<ETCInterfaceType>(0x33);
constexpr ETCInterfaceType kOwnerPhysicalInterfaceType = static_cast<ETCInterfaceType>(0x23);
constexpr ETCInterfaceType kTeleportInterfaceType = static_cast<ETCInterfaceType>(0xD5);
constexpr ETCInterfaceType kHeroOpinionLogInterfaceType = static_cast<ETCInterfaceType>(0x51);

constexpr std::uint32_t kChestFlag24 = 0x10000000;
constexpr std::uint32_t kHeroTutorialFlag24 = 0x00000200;
constexpr std::uint32_t kOwnedChestFlag24 = 0x00080000;
constexpr std::uint32_t kRelevantOwnerFlag34 = 0x20000000;
constexpr std::uint32_t kRelevantOwnerFlag28 = 0x00080000;
constexpr std::uint32_t kTeleportFlag38 = 0x00200000;
constexpr std::uint32_t kHeroOpinionFlag28 = 0x00020000;
constexpr std::uint8_t kDeadFlag91Mask = 0x01;
constexpr std::uint8_t kOwnerMaskAt24 = 0x08;
constexpr std::size_t kTutorialGateOffsetF6 = 0xF6;

constexpr ETutorialCategory kOpenChestTutorialCategory =
    static_cast<ETutorialCategory>(0x21);
constexpr EOpinionDeedType kOpenChestDeedType =
    static_cast<EOpinionDeedType>(6);
constexpr EOpinionPostDeedType kOpenChestPostDeedType =
    static_cast<EOpinionPostDeedType>(2);

template <typename TObject>
CKeyPair<ETCInterfaceType, CTCBase*>* FindInterface(TObject* pObject, ETCInterfaceType type)
{
    auto* const pOverlay = reinterpret_cast<CThingOverlay*>(pObject);
    CKeyPair<ETCInterfaceType, CTCBase*>* pEntry = pOverlay->m_InterfaceMap44.LowerBound(&type);
    if ((pEntry == pOverlay->m_InterfaceMap44.m_pEnd) ||
        (static_cast<std::int32_t>(type) < static_cast<std::int32_t>(pEntry->m_Key)))
    {
        pEntry = pOverlay->m_InterfaceMap44.m_pEnd;
    }
    return pEntry;
}

#if defined(_M_IX86)
[[nodiscard]] RegisterPair CallCatch_0040D959(void* pThis)
{
    RegisterPair result;
    __asm
    {
        mov ecx, pThis
        mov eax, Catch_0040D959
        call eax
        mov result.m_Eax, eax
        mov result.m_Edx, edx
    }
    return result;
}

[[nodiscard]] RegisterPair CallConsValWithLiveEdi(void* pThis, void* pEdxArgument, void* pLiveEdi)
{
    RegisterPair result;
    __asm
    {
        mov ecx, pThis
        mov edx, pEdxArgument
        mov edi, pLiveEdi
        mov eax, kConsValAddress
        call eax
        mov result.m_Eax, eax
        mov result.m_Edx, edx
    }
    return result;
}

[[nodiscard]] RegisterPair CallPhysicsMeshInfo___CPhysicsMeshInfo(void* pThis)
{
    RegisterPair result;
    __asm
    {
        mov ecx, pThis
        mov eax, CPhysicsMeshInfo___CPhysicsMeshInfo
        call eax
        mov result.m_Eax, eax
        mov result.m_Edx, edx
    }
    return result;
}

inline void CallConsValWithLiveEsi(void* pThis, void* pEdxArgument, void* pLiveEsi)
{
    __asm
    {
        mov ecx, pThis
        mov edx, pEdxArgument
        mov esi, pLiveEsi
        mov eax, kConsValAddress
        call eax
    }
}
#else
#error This reverse depends on the original x86 register ABI.
#endif
} // namespace

bool __thiscall CGameScriptInterface::OpenChest(const CScriptThing& thing, bool useHero) const
{
    const auto* const pScriptThing = reinterpret_cast<const CScriptThingOverlay*>(&thing);

    if (!pScriptThing->m_pVTable->m_RawSlot12C(&thing))
    {
        return false;
    }

    CThing* const pThing = pScriptThing->m_pVTable->m_GetThing(&thing);
    auto* const pThingOverlay = reinterpret_cast<CThingOverlay*>(pThing);
    if ((pThingOverlay->m_Flags24 & kChestFlag24) == 0)
    {
        return false;
    }

    CKeyPair<ETCInterfaceType, CTCBase*>* pEntry = FindInterface(pThing, kChestInterfaceType);
    auto* const pOccupiable = reinterpret_cast<CTCSinglePersonOccupiable*>(pEntry->m_Value);

    if (!useHero)
    {
        reinterpret_cast<CTCChest*>(pOccupiable)->Open();
        return true;
    }

    const auto* const pThisOverlay = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
    const RegisterPair catchResult = CallCatch_0040D959(pThisOverlay->m_Field14);
    const RegisterPair firstConsValResult =
        CallConsValWithLiveEdi(catchResult.m_Eax, catchResult.m_Edx, &pThingOverlay->m_InterfaceMap44);
    auto* const pHero = reinterpret_cast<CThingCreatureBase*>(firstConsValResult.m_Eax);

    if ((pHero == nullptr) ||
        ((reinterpret_cast<CThingOverlay*>(pHero)->m_Flags91 & kDeadFlag91Mask) != 0))
    {
        return false;
    }

    auto* const pHeroOverlay = reinterpret_cast<CThingOverlay*>(pHero);
    if ((*reinterpret_cast<std::uint8_t*>(
             reinterpret_cast<std::byte*>(DAT_013B8790) + kTutorialGateOffsetF6) == 0) &&
        ((pHeroOverlay->m_Flags24 & kHeroTutorialFlag24) != 0))
    {
        pEntry = FindInterface(pHero, kHeroInterfaceType);
        if (reinterpret_cast<CTCHero*>(pEntry->m_Value)
                ->GiveTutorialForCategory(kOpenChestTutorialCategory, false))
        {
            return false;
        }
    }

    pHero->FinishCurrentAction();

    EOccupyType occupyType;
    CCreatureAction_GetOutOfBed_Storage actionStorage;
    CCreatureActionBase* const pAction =
        reinterpret_cast<CCreatureActionBase*>(
            new (&actionStorage)
                CCreatureAction_GetOutOfBed(
                    pHero,
                    pThing,
                    pOccupiable->IsOfType(&occupyType)));

    const bool actionSet = pHero->SetCurrentAction(pAction);

    CBaseIntelligentPointer_Storage pointerStorage;
    reinterpret_cast<CBaseIntelligentPointer*>(&pointerStorage)->~CBaseIntelligentPointer();

    RawPhysicsMeshInfoStorage physicsMeshInfoStorage;
    RawStackSlot consValStackSlot;
    const RegisterPair physicsMeshInfoResult =
        CallPhysicsMeshInfo___CPhysicsMeshInfo(&physicsMeshInfoStorage);
    CallConsValWithLiveEsi(&consValStackSlot, physicsMeshInfoResult.m_Edx, pThing);

    if (!actionSet)
    {
        return false;
    }

    if ((pThingOverlay->m_Flags24 & kOwnedChestFlag24) != 0)
    {
        pEntry = FindInterface(pThing, kOwnerInterfaceType);
        auto* const pInterface33 = reinterpret_cast<CInterface33Overlay*>(pEntry->m_Value);
        CBaseIntelligentPointer* const pOwnerPointer = &pInterface33->m_Pointer0D;

        CThingBuilding* pOwnedBuilding =
            reinterpret_cast<CThingBuilding*>(CBaseIntelligentPointer::GetPItem(pOwnerPointer));
        if (IS_THING_ALIVE(&pOwnedBuilding))
        {
            CBaseObject* const pOwnerAt24 = CBaseIntelligentPointer::GetPItem(pOwnerPointer);
            if ((reinterpret_cast<const std::uint8_t*>(pOwnerAt24)[0x24] & kOwnerMaskAt24) != 0)
            {
                pEntry = FindInterface(pOwnerAt24, kOwnerPhysicalInterfaceType);
                if (IS_THING_ALIVE(
                        reinterpret_cast<CIntelligentPointer<const CThingPhysical>*>(pEntry->m_Value)))
                {
                    CBaseObject* pOwnerFlags = CBaseIntelligentPointer::GetPItem(pOwnerPointer);
                    if (((reinterpret_cast<CThingOverlay*>(pOwnerFlags)->m_Flags34 &
                          kRelevantOwnerFlag34) != 0) ||
                        ((reinterpret_cast<CThingOverlay*>(
                              CBaseIntelligentPointer::GetPItem(pOwnerPointer))
                              ->m_Flags28 &
                          kRelevantOwnerFlag28) != 0))
                    {
                        pOwnerFlags = CBaseIntelligentPointer::GetPItem(pOwnerPointer);
                        if ((reinterpret_cast<CThingOverlay*>(pOwnerFlags)->m_Flags38 &
                             kTeleportFlag38) != 0)
                        {
                            pEntry = FindInterface(pOwnerFlags, kTeleportInterfaceType);
                            if (reinterpret_cast<CInterfaceD5Overlay*>(pEntry->m_Value)->m_Field11 != 0)
                            {
                                return true;
                            }
                        }

                        if ((pHeroOverlay->m_Flags28 & kHeroOpinionFlag28) != 0)
                        {
                            pEntry = FindInterface(pHero, kHeroOpinionLogInterfaceType);
                            reinterpret_cast<CTCHeroOpinionDeedLog*>(pEntry->m_Value)
                                ->PostDeed(
                                    kOpenChestDeedType,
                                    CVertexBufferWin32::DoSizeof(
                                        reinterpret_cast<CVertexBufferWin32*>(pHero)),
                                    pThing,
                                    nullptr,
                                    kOpenChestPostDeedType,
                                    true,
                                    false,
                                    nullptr,
                                    false,
                                    0);
                        }
                    }
                }
            }
        }
    }

    return true;
}