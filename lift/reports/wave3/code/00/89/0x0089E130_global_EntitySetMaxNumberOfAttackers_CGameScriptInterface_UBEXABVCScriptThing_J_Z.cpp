#include <cstddef>
#include <cstdint>

class CScriptThing;
class CTCBase;

class CGameScriptInterface
{
public:
    void __thiscall EntitySetMaxNumberOfAttackers(const CScriptThing& entity, long max_number_of_attackers) const;
};

namespace
{
using CScriptThing_GetThingFn = void* (__thiscall*)(const CScriptThing*);

struct CScriptThing_VTable_Overlay
{
    std::byte m_Pad00[0x2C];
    CScriptThing_GetThingFn m_GetThing; // 0x2C
};

static_assert(offsetof(CScriptThing_VTable_Overlay, m_GetThing) == 0x2C);

enum ETCInterfaceType : std::int32_t
{
};

template <typename TKey, typename TValue>
struct CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    std::int32_t* __thiscall LowerBound(const TKey* key);
};

struct EntitySetMaxNumberOfAttackers_Target_Overlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24; // 0x24
    std::byte m_Pad28[0x44 - 0x28];
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>> m_InterfaceMap; // 0x44
    std::int32_t* m_InterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x91 - 0x4C];
    std::uint8_t m_Byte91; // 0x91
};

static_assert(offsetof(EntitySetMaxNumberOfAttackers_Target_Overlay, m_Flags24) == 0x24);
static_assert(offsetof(EntitySetMaxNumberOfAttackers_Target_Overlay, m_InterfaceMap) == 0x44);
static_assert(offsetof(EntitySetMaxNumberOfAttackers_Target_Overlay, m_InterfaceMapEnd) == 0x48);
static_assert(offsetof(EntitySetMaxNumberOfAttackers_Target_Overlay, m_Byte91) == 0x91);

struct EntitySetMaxNumberOfAttackers_InterfaceEntryValue_Overlay
{
    std::byte m_Pad00[0x44];
    std::int32_t m_MaxNumberOfAttackers; // 0x44
};

static_assert(offsetof(EntitySetMaxNumberOfAttackers_InterfaceEntryValue_Overlay, m_MaxNumberOfAttackers) == 0x44);
}

void __thiscall CGameScriptInterface::EntitySetMaxNumberOfAttackers(
    const CScriptThing& entity,
    long max_number_of_attackers) const
{
    const auto* const vtable = *reinterpret_cast<CScriptThing_VTable_Overlay* const*>(&entity);
    auto* const target =
        static_cast<EntitySetMaxNumberOfAttackers_Target_Overlay*>((vtable->m_GetThing)(&entity));

    if ((target != nullptr) &&
        ((target->m_Byte91 & 1) == 0) &&
        ((target->m_Flags24 & 0x400000) != 0))
    {
        const auto interface_type = static_cast<ETCInterfaceType>(0x36);
        std::int32_t* entry = target->m_InterfaceMap.LowerBound(&interface_type);

        if ((entry == target->m_InterfaceMapEnd) || (0x36 < *entry))
        {
            entry = target->m_InterfaceMapEnd;
        }

        auto* const interface_value =
            reinterpret_cast<EntitySetMaxNumberOfAttackers_InterfaceEntryValue_Overlay*>(entry[1]);
        interface_value->m_MaxNumberOfAttackers = max_number_of_attackers;
    }
}