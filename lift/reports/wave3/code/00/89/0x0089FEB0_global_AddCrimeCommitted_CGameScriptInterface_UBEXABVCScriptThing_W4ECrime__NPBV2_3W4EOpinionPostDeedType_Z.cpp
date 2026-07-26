#include <cstddef>
#include <cstdint>

class C3DVector;
class CBaseIntelligentPointer;
class CBaseObject;
class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCCreatureOpinionOfHero;
class CTCHeroOpinionDeedLog;
class CFaction;
class CThing;
class CThingAICreature;

enum ECrime : std::int32_t;
enum ETCInterfaceType : std::int32_t;
enum EOpinionPostDeedType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    static void* __thiscall LowerBound(CVectorMap* pThis, const TKey* pKey);
};

template <typename TChar>
class CBasicString;

template <>
class CBasicString<char>
{
public:
    static std::size_t __thiscall CalcStringLength(const char* pThis);
};

class CBaseIntelligentPointer
{
public:
    static CBaseObject* __thiscall GetPItem(CBaseIntelligentPointer* pThis);
};

class CThing
{
public:
    C3DVector* __thiscall DoSizeof() const;
};

class CTCHeroOpinionDeedLog
{
public:
    std::int32_t __thiscall PostDeed(
        ECrime crime,
        const C3DVector* pPosition,
        CThing* pThing,
        CThingAICreature* pCreature,
        EOpinionPostDeedType postDeedType,
        bool param6,
        bool param7,
        CFaction* pFaction,
        bool param9,
        long param10);
};

class CTCCreatureOpinionOfHero
{
public:
    bool __thiscall KnowsOfDeed(long deedId, ECrime crime, bool param3);
    void __thiscall InformOfDeed(std::int32_t deedId, bool param2);
};

class CGameScriptInterface
{
public:
    void __thiscall AddCrimeCommitted(
        const CScriptThing& thing,
        ECrime crime,
        bool informWitnesses,
        const CScriptThing* pThing,
        const CScriptThing* pCreature,
        EOpinionPostDeedType postDeedType) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_AddCrimeCommitted_SelfOverlay
{
    std::byte pad00[0x14];
    void* field14; // +0x14
};
static_assert(offsetof(CGameScriptInterface_AddCrimeCommitted_SelfOverlay, field14) == 0x14);

struct CGameScriptInterface_AddCrimeCommitted_MapEntryOverlay
{
    std::int32_t key; // +0x00
    CTCBase* value;   // +0x04
};
static_assert(offsetof(CGameScriptInterface_AddCrimeCommitted_MapEntryOverlay, value) == 0x04);

struct CGameScriptInterface_AddCrimeCommitted_ScriptThingOverlay
{
    void** vftable; // +0x00
};
static_assert(offsetof(CGameScriptInterface_AddCrimeCommitted_ScriptThingOverlay, vftable) == 0x00);

struct CGameScriptInterface_AddCrimeCommitted_ObjectOverlay
{
    std::byte pad00[0x24];
    std::uint8_t flags24; // +0x24
    std::byte pad25[0x03];
    std::uint32_t flags28; // +0x28
    std::byte pad2C[0x08];
    std::uint32_t flags34; // +0x34
    std::byte pad38[0x0C];
    std::byte interfaceMapStorage[0x04]; // +0x44
    CGameScriptInterface_AddCrimeCommitted_MapEntryOverlay* interfaceMapEnd; // +0x48
    std::byte pad4C[0x20];
    std::uint8_t flags6C; // +0x6C
    std::byte pad6D[0x24];
    std::uint8_t flags91; // +0x91
};
static_assert(offsetof(CGameScriptInterface_AddCrimeCommitted_ObjectOverlay, flags24) == 0x24);
static_assert(offsetof(CGameScriptInterface_AddCrimeCommitted_ObjectOverlay, flags28) == 0x28);
static_assert(offsetof(CGameScriptInterface_AddCrimeCommitted_ObjectOverlay, flags34) == 0x34);
static_assert(offsetof(CGameScriptInterface_AddCrimeCommitted_ObjectOverlay, interfaceMapStorage) == 0x44);
static_assert(offsetof(CGameScriptInterface_AddCrimeCommitted_ObjectOverlay, interfaceMapEnd) == 0x48);
static_assert(offsetof(CGameScriptInterface_AddCrimeCommitted_ObjectOverlay, flags6C) == 0x6C);
static_assert(offsetof(CGameScriptInterface_AddCrimeCommitted_ObjectOverlay, flags91) == 0x91);

struct CGameScriptInterface_AddCrimeCommitted_ListNodeOverlay
{
    CGameScriptInterface_AddCrimeCommitted_ListNodeOverlay* next; // +0x00
    CGameScriptInterface_AddCrimeCommitted_ListNodeOverlay* prev; // +0x04
    CBaseIntelligentPointer item; // +0x08
};
static_assert(offsetof(CGameScriptInterface_AddCrimeCommitted_ListNodeOverlay, item) == 0x08);

struct CGameScriptInterface_AddCrimeCommitted_Interface22ListOverlay
{
    std::byte pad00[0x2C];
    CGameScriptInterface_AddCrimeCommitted_ListNodeOverlay* head; // +0x2C
};
static_assert(offsetof(CGameScriptInterface_AddCrimeCommitted_Interface22ListOverlay, head) == 0x2C);

using Fn_0040D959 = void* (__thiscall*)(void*);
using Fn_00487DC0 = void* (__thiscall*)(void*);
using ScriptThingBoolSlotFn =
    bool (__thiscall*)(const CGameScriptInterface_AddCrimeCommitted_ScriptThingOverlay*);
using ScriptThingResolveSlotFn =
    void* (__thiscall*)(const CGameScriptInterface_AddCrimeCommitted_ScriptThingOverlay*);

inline constexpr Fn_0040D959 CALL_0040D959 =
    reinterpret_cast<Fn_0040D959>(0x0040D959);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);

inline bool ScriptThingCallSlot300(const CScriptThing& thing)
{
    const auto* const overlay =
        reinterpret_cast<const CGameScriptInterface_AddCrimeCommitted_ScriptThingOverlay*>(&thing);
    const auto fn = reinterpret_cast<ScriptThingBoolSlotFn>(overlay->vftable[0x12C / 4]);
    return fn(overlay);
}

inline void* ScriptThingCallSlot2C(const CScriptThing& thing)
{
    const auto* const overlay =
        reinterpret_cast<const CGameScriptInterface_AddCrimeCommitted_ScriptThingOverlay*>(&thing);
    const auto fn = reinterpret_cast<ScriptThingResolveSlotFn>(overlay->vftable[0x2C / 4]);
    return fn(overlay);
}
}

void __thiscall CGameScriptInterface::AddCrimeCommitted(
    const CScriptThing& thing,
    ECrime crime,
    bool informWitnesses,
    const CScriptThing* pThing,
    const CScriptThing* pCreature,
    EOpinionPostDeedType postDeedType) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_AddCrimeCommitted_SelfOverlay*>(this);

    void* const catchResult = CALL_0040D959(self->field14);
    auto* const target =
        reinterpret_cast<CGameScriptInterface_AddCrimeCommitted_ObjectOverlay*>(
            CALL_00487DC0(catchResult));

    if ((target == nullptr) || ((target->flags91 & 0x01) != 0) || !ScriptThingCallSlot300(thing))
    {
        return;
    }

    CThing* relatedThing = nullptr;
    if (pThing != nullptr)
    {
        relatedThing = reinterpret_cast<CThing*>(ScriptThingCallSlot2C(*pThing));
    }

    CBaseObject* relatedCreatureObject = nullptr;
    if ((pCreature != nullptr) && ScriptThingCallSlot300(*pCreature))
    {
        auto* const resolvedCreature =
            reinterpret_cast<CGameScriptInterface_AddCrimeCommitted_ObjectOverlay*>(
                ScriptThingCallSlot2C(*pCreature));
        if ((resolvedCreature->flags6C & 0x10) != 0)
        {
            relatedCreatureObject = reinterpret_cast<CBaseObject*>(ScriptThingCallSlot2C(*pCreature));
        }
    }

    std::int32_t deedId = -1;

    if (informWitnesses && (relatedCreatureObject == nullptr))
    {
        auto* const resolvedThing =
            reinterpret_cast<CGameScriptInterface_AddCrimeCommitted_ObjectOverlay*>(
                ScriptThingCallSlot2C(thing));
        if ((resolvedThing->flags24 & 0x04) != 0)
        {
            ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x22);
            auto* entry =
                reinterpret_cast<CGameScriptInterface_AddCrimeCommitted_MapEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(resolvedThing->interfaceMapStorage),
                        &interfaceType));

            if ((entry == resolvedThing->interfaceMapEnd) || (0x22 < entry->key))
            {
                entry = resolvedThing->interfaceMapEnd;
            }

            auto* const interface22 = reinterpret_cast<std::byte*>(entry->value);
            auto* const field2C = interface22 + 0x2C;

            if (CBasicString<char>::CalcStringLength(reinterpret_cast<const char*>(field2C)) != 0)
            {
                relatedCreatureObject = CBaseIntelligentPointer::GetPItem(
                    reinterpret_cast<CBaseIntelligentPointer*>(
                        *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<void**>(field2C)) + 8));
            }
        }
    }

    if ((target->flags28 & 0x00020000) != 0)
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x51);
        auto* entry =
            reinterpret_cast<CGameScriptInterface_AddCrimeCommitted_MapEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->interfaceMapStorage),
                    &interfaceType));

        if ((entry == target->interfaceMapEnd) || (0x51 < entry->key))
        {
            entry = target->interfaceMapEnd;
        }

        auto* const deedLog = reinterpret_cast<CTCHeroOpinionDeedLog*>(entry->value);
        deedId = deedLog->PostDeed(
            crime,
            reinterpret_cast<CThing*>(target)->DoSizeof(),
            relatedThing,
            reinterpret_cast<CThingAICreature*>(relatedCreatureObject),
            postDeedType,
            true,
            false,
            nullptr,
            false,
            0);
    }

    if (informWitnesses && (deedId != -1) && ScriptThingCallSlot300(thing))
    {
        auto* const resolvedThing =
            reinterpret_cast<CGameScriptInterface_AddCrimeCommitted_ObjectOverlay*>(
                ScriptThingCallSlot2C(thing));
        if ((resolvedThing->flags24 & 0x04) != 0)
        {
            ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x22);
            auto* entry =
                reinterpret_cast<CGameScriptInterface_AddCrimeCommitted_MapEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(resolvedThing->interfaceMapStorage),
                        &interfaceType));

            if ((entry == resolvedThing->interfaceMapEnd) || (0x22 < entry->key))
            {
                entry = resolvedThing->interfaceMapEnd;
            }

            auto* const interface22 =
                reinterpret_cast<CGameScriptInterface_AddCrimeCommitted_Interface22ListOverlay*>(
                    entry->value);
            auto* node = interface22->head->next;

            while (node != interface22->head)
            {
                auto* const watcher = CBaseIntelligentPointer::GetPItem(&node->item);
                if (watcher != nullptr)
                {
                    auto* const watcherOverlay =
                        reinterpret_cast<CGameScriptInterface_AddCrimeCommitted_ObjectOverlay*>(
                            watcher);
                    if (((watcherOverlay->flags91 & 0x01) == 0) &&
                        ((watcherOverlay->flags34 & 0x00100000) != 0) &&
                        (static_cast<char>(watcherOverlay->flags28 >> 8) < 0))
                    {
                        ETCInterfaceType creatureInterfaceType = static_cast<ETCInterfaceType>(0x4F);
                        auto* creatureEntry =
                            reinterpret_cast<CGameScriptInterface_AddCrimeCommitted_MapEntryOverlay*>(
                                TCInterfaceMap::LowerBound(
                                    reinterpret_cast<TCInterfaceMap*>(watcherOverlay->interfaceMapStorage),
                                    &creatureInterfaceType));

                        if ((creatureEntry == watcherOverlay->interfaceMapEnd) ||
                            (0x4F < creatureEntry->key))
                        {
                            creatureEntry = watcherOverlay->interfaceMapEnd;
                        }

                        auto* const opinion =
                            reinterpret_cast<CTCCreatureOpinionOfHero*>(creatureEntry->value);
                        if (!opinion->KnowsOfDeed(static_cast<long>(deedId), crime, true))
                        {
                            opinion->InformOfDeed(deedId, false);
                        }
                    }
                }

                node = node->next;
            }
        }
    }
}