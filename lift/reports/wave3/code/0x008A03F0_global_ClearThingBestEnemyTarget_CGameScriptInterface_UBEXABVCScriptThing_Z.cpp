#include <cstddef>
#include <cstdint>
#include <new>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CThing;
class CVillageTaskCarryCrate;

enum ETCInterfaceType : std::int32_t
{
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    void* __thiscall LowerBound(const TKey* pKey);
};

template <typename TChar>
class CBasicString;

template <>
class CBasicString<char>
{
public:
    static long __fastcall Compare(const char* pLeft, const char* pRight);
};

struct CCharStringRep
{
    const char* m_pString; // 0x00
    std::int32_t m_Length; // 0x04
};

static_assert(offsetof(CCharStringRep, m_pString) == 0x00);
static_assert(offsetof(CCharStringRep, m_Length) == 0x04);
static_assert(sizeof(CCharStringRep) == 0x08);

class CCharString
{
public:
    CCharStringRep* m_pRep; // 0x00

    CCharString(const char* pString, long length);
    ~CCharString();
};

static_assert(offsetof(CCharString, m_pRep) == 0x00);
static_assert(sizeof(CCharString) == 0x04);

class CVillageTaskCarryCrate
{
public:
    void __thiscall AssignCreatureToTask(CThing* pThing);
};

class CGameScriptInterface
{
public:
    void __thiscall ClearThingBestEnemyTarget(const CScriptThing& thing) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CScriptThingOverlay
{
    void** m_pVftable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVftable) == 0x00);

struct TCInterfaceEntryOverlay
{
    std::int32_t m_Key; // 0x00
    CTCBase* m_pInterface; // 0x04
};

static_assert(offsetof(TCInterfaceEntryOverlay, m_Key) == 0x00);
static_assert(offsetof(TCInterfaceEntryOverlay, m_pInterface) == 0x04);
static_assert(sizeof(TCInterfaceEntryOverlay) == 0x08);

struct TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_Flags20; // 0x20
    std::byte m_Pad24[0x20];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    TCInterfaceEntryOverlay* m_pInterfaceMapEnd; // 0x48
};

static_assert(offsetof(TargetOverlay, m_Flags20) == 0x20);
static_assert(offsetof(TargetOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(TargetOverlay, m_pInterfaceMapEnd) == 0x48);

using CheckThingFn = char(__thiscall*)(const CScriptThing*);
using GetTargetFn = TargetOverlay*(__thiscall*)(const CScriptThing*);
using GetTaskTypeStringFn = CCharString*(__thiscall*)(CVillageTaskCarryCrate*, CCharString*);
}

void __thiscall CGameScriptInterface::ClearThingBestEnemyTarget(const CScriptThing& thing) const
{
    const auto* const thingOverlay = reinterpret_cast<const CScriptThingOverlay*>(&thing);

    const auto checkThing =
        reinterpret_cast<CheckThingFn>(thingOverlay->m_pVftable[0x12C / sizeof(void*)]);
    if (checkThing(&thing) == '\0')
    {
        return;
    }

    const auto getTarget =
        reinterpret_cast<GetTargetFn>(thingOverlay->m_pVftable[0x2C / sizeof(void*)]);
    auto* const target = getTarget(&thing);
    if ((target->m_Flags20 & 0x00000100) == 0)
    {
        return;
    }

    std::int32_t interfaceTypeValue = 8;
    auto* pEntry = reinterpret_cast<TCInterfaceEntryOverlay*>(
        target->m_InterfaceMap44.LowerBound(reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));
    if ((pEntry == target->m_pInterfaceMapEnd) || (8 < pEntry->m_Key))
    {
        pEntry = target->m_pInterfaceMapEnd;
    }

    auto* const pCarryCrateTask = reinterpret_cast<CVillageTaskCarryCrate*>(pEntry->m_pInterface);

    alignas(CCharString) std::byte targetingAIStorage[sizeof(CCharString)];
    auto* const pTargetingAI = new (targetingAIStorage) CCharString("CTCTargetingAI", -1);

    alignas(CCharString) std::byte currentTaskTypeStorage[sizeof(CCharString)];
    auto* const pCurrentTaskType = reinterpret_cast<CCharString*>(currentTaskTypeStorage);

    const auto getTaskTypeString =
        reinterpret_cast<GetTaskTypeStringFn>((*reinterpret_cast<void***>(pCarryCrateTask))[0x58 / sizeof(void*)]);
    getTaskTypeString(pCarryCrateTask, pCurrentTaskType);

    char matches;
    if (pCurrentTaskType->m_pRep == pTargetingAI->m_pRep)
    {
        matches = '\x01';
    }
    else if ((pCurrentTaskType->m_pRep == nullptr) || (pTargetingAI->m_pRep == nullptr))
    {
        matches = '\0';
    }
    else if (pCurrentTaskType->m_pRep->m_Length == pTargetingAI->m_pRep->m_Length)
    {
        matches = static_cast<char>(
            1 - (CBasicString<char>::Compare(
                     pCurrentTaskType->m_pRep->m_pString,
                     pTargetingAI->m_pRep->m_pString) != 0));
    }
    else
    {
        matches = '\0';
    }

    pCurrentTaskType->~CCharString();
    pTargetingAI->~CCharString();

    if (matches != '\0')
    {
        pCarryCrateTask->AssignCreatureToTask(nullptr);
    }
}