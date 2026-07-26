#include <cstddef>
#include <cstdint>

class CScriptThing;
class CTCBase;

enum EOpinionAttitudeType : std::int32_t
{
};

class CGameScriptInterface
{
public:
    void __thiscall EntitySetOpinionAttitudeEnabled(
        const CScriptThing& entity,
        EOpinionAttitudeType opinion_attitude_type,
        bool enabled) const;
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
struct CKeyPair
{
    TKey m_Key;
    TValue m_Value;
};

static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Key) == 0x00);
static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Value) == 0x04);
static_assert(sizeof(CKeyPair<ETCInterfaceType, CTCBase*>) == 0x08);

template <typename TKey, typename TValue>
struct CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* key);
};

struct EntitySetOpinionAttitudeEnabled_Target_Overlay
{
    std::byte m_Pad00[0x29];
    std::int8_t m_Byte29; // 0x29
    std::byte m_Pad2A[0x44 - 0x2A];
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>> m_InterfaceMap; // 0x44
    CKeyPair<ETCInterfaceType, CTCBase*>* m_InterfaceMapEnd; // 0x48
};

static_assert(offsetof(EntitySetOpinionAttitudeEnabled_Target_Overlay, m_Byte29) == 0x29);
static_assert(offsetof(EntitySetOpinionAttitudeEnabled_Target_Overlay, m_InterfaceMap) == 0x44);
static_assert(offsetof(EntitySetOpinionAttitudeEnabled_Target_Overlay, m_InterfaceMapEnd) == 0x48);

struct EntitySetOpinionAttitudeEnabled_InterfaceValue_Overlay
{
    std::byte m_Pad00[0x24];
    std::uint8_t* m_AttitudeEnabledArray; // 0x24
};

static_assert(offsetof(EntitySetOpinionAttitudeEnabled_InterfaceValue_Overlay, m_AttitudeEnabledArray) == 0x24);
}

void __thiscall CGameScriptInterface::EntitySetOpinionAttitudeEnabled(
    const CScriptThing& entity,
    EOpinionAttitudeType opinion_attitude_type,
    bool enabled) const
{
    const auto* const vtable = *reinterpret_cast<CScriptThing_VTable_Overlay* const*>(&entity);
    auto* const target =
        static_cast<EntitySetOpinionAttitudeEnabled_Target_Overlay*>((vtable->m_GetThing)(&entity));

    if ((target != nullptr) && (target->m_Byte29 < 0))
    {
        const auto interface_type = static_cast<ETCInterfaceType>(0x4F);
        CKeyPair<ETCInterfaceType, CTCBase*>* entry = target->m_InterfaceMap.LowerBound(&interface_type);

        if ((entry == target->m_InterfaceMapEnd) || (static_cast<ETCInterfaceType>(0x4F) < entry->m_Key))
        {
            entry = target->m_InterfaceMapEnd;
        }

        auto* const interface_value =
            reinterpret_cast<EntitySetOpinionAttitudeEnabled_InterfaceValue_Overlay*>(entry->m_Value);
        interface_value->m_AttitudeEnabledArray[static_cast<std::int32_t>(opinion_attitude_type)] =
            static_cast<std::uint8_t>(enabled);
    }
}