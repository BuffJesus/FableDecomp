#include <cstddef>
#include <cstdint>

struct TcInterfaceMapStorage
{
    std::byte storage[0x4];
};
static_assert(sizeof(TcInterfaceMapStorage) == 0x4);

struct TcInterfaceEntry
{
    ETCInterfaceType key;
    CTCBase* value;
};
static_assert(offsetof(TcInterfaceEntry, key) == 0x0);
static_assert(offsetof(TcInterfaceEntry, value) == 0x4);
static_assert(sizeof(TcInterfaceEntry) == 0x8);

struct EntityShotStrikeOverlay
{
    std::byte pad00[0x28];
    std::uint8_t flags28;
    std::byte pad29[0x1B];
    TcInterfaceMapStorage tcInterfaceMap;
    TcInterfaceEntry* tcInterfaceMapEnd;
    std::byte pad4C[0x45];
    std::uint8_t flags91;
};
static_assert(offsetof(EntityShotStrikeOverlay, flags28) == 0x28);
static_assert(offsetof(EntityShotStrikeOverlay, tcInterfaceMap) == 0x44);
static_assert(offsetof(EntityShotStrikeOverlay, tcInterfaceMapEnd) == 0x48);
static_assert(offsetof(EntityShotStrikeOverlay, flags91) == 0x91);

struct EventHitByLookupBuffer
{
    ETCInterfaceType interfaceType;
    std::byte pad04[0x4];
    C3DVector strikePos;
};
static_assert(offsetof(EventHitByLookupBuffer, interfaceType) == 0x0);
static_assert(offsetof(EventHitByLookupBuffer, strikePos) == 0x8);

bool CGameScriptInterface::EntityGetShotStrikePos(const CScriptThing& scriptThing, C3DVector& outStrikePos) const
{
    using RawEntityGetter = void* (__thiscall*)(const CScriptThing*);
    auto* const* const vftable = *reinterpret_cast<void* const* const*>(&scriptThing);
    auto* const entity = static_cast<EntityShotStrikeOverlay*>(
        reinterpret_cast<RawEntityGetter>(vftable[0x2C / sizeof(void*)])(&scriptThing));

    if (entity != nullptr &&
        (entity->flags91 & 0x01) == 0 &&
        (entity->flags28 & 0x04) != 0)
    {
        EventHitByLookupBuffer event;
        event.interfaceType = static_cast<ETCInterfaceType>(0x42);

        TcInterfaceEntry* entry =
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>::LowerBound(
                reinterpret_cast<CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>*>(
                    &entity->tcInterfaceMap),
                &event.interfaceType);

        if (entry == entity->tcInterfaceMapEnd || static_cast<ETCInterfaceType>(0x42) < entry->key)
        {
            entry = entity->tcInterfaceMapEnd;
        }

        if (CTCEntityEvents::GetEvent<NEntityEvents::CEventHitBy const, NEntityEvents::CEventFilter_None>(
                static_cast<CTCEntityEvents*>(entry->value),
                &event,
                reinterpret_cast<NEntityEvents::CEventFilter_None*>(&event)))
        {
            outStrikePos.x = event.strikePos.x;
            outStrikePos.y = event.strikePos.y;
            outStrikePos.z = event.strikePos.z;
            return true;
        }
    }

    return false;
}