#include <cstddef>
#include <cstdint>

class CScriptThing;
class CTCBase;

enum EOpinionDeedType : std::int32_t
{
};

class CGameScriptInterface
{
public:
    void __thiscall EntitySetOpinionDeedTypeEnabled(
        const CScriptThing& entity,
        EOpinionDeedType opinion_deed_type,
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
    ETCInterfaceType_OpinionDeed = 0x4F
};

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap;

template <typename TKey, typename TValue>
struct CKeyPairCompareLess;

struct CVectorMapNode_Overlay
{
    ETCInterfaceType m_Key; // 0x00
    CTCBase* m_Value;       // 0x04
};

static_assert(offsetof(CVectorMapNode_Overlay, m_Key) == 0x00);
static_assert(offsetof(CVectorMapNode_Overlay, m_Value) == 0x04);

using OpinionDeedInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct OpinionDeedInterfaceMap_Overlay
{
    std::byte m_Pad00[0x04];
    CVectorMapNode_Overlay* m_End; // 0x04
};

static_assert(offsetof(OpinionDeedInterfaceMap_Overlay, m_End) == 0x04);

struct CThingWithOpinionInterfaces_Overlay
{
    std::byte m_Pad00[0x28];
    std::uint32_t m_Flags28; // 0x28
    std::byte m_Pad2C[0x44 - 0x2C];
    OpinionDeedInterfaceMap_Overlay m_Interfaces; // 0x44
};

static_assert(offsetof(CThingWithOpinionInterfaces_Overlay, m_Flags28) == 0x28);
static_assert(offsetof(CThingWithOpinionInterfaces_Overlay, m_Interfaces) == 0x44);

struct COpinionDeedTCBase_Overlay
{
    std::byte m_Pad00[0x3C];
    std::int32_t m_DeedTypeFlagsOffset; // 0x3C
};

static_assert(offsetof(COpinionDeedTCBase_Overlay, m_DeedTypeFlagsOffset) == 0x3C);
}

void __thiscall CGameScriptInterface::EntitySetOpinionDeedTypeEnabled(
    const CScriptThing& entity,
    EOpinionDeedType opinion_deed_type,
    bool enabled) const
{
    const auto* const entity_vtable =
        *reinterpret_cast<const CScriptThing_VTable_Overlay* const*>(&entity);
    auto* const thing = static_cast<CThingWithOpinionInterfaces_Overlay*>(
        entity_vtable->m_GetThing(&entity));

    if ((thing != nullptr) && (static_cast<std::int8_t>(thing->m_Flags28 >> 8) < 0))
    {
        ETCInterfaceType interface_type = ETCInterfaceType_OpinionDeed;

        auto* interface_it = reinterpret_cast<CVectorMapNode_Overlay*>(
            OpinionDeedInterfaceMap::LowerBound(
                reinterpret_cast<OpinionDeedInterfaceMap*>(&thing->m_Interfaces),
                &interface_type));

        if ((interface_it == thing->m_Interfaces.m_End) ||
            (ETCInterfaceType_OpinionDeed < interface_it->m_Key))
        {
            interface_it = thing->m_Interfaces.m_End;
        }

        const auto* const opinion_deed_interface =
            reinterpret_cast<const COpinionDeedTCBase_Overlay*>(interface_it->m_Value);

        *reinterpret_cast<std::uint8_t*>(
            static_cast<std::int32_t>(opinion_deed_type) +
            opinion_deed_interface->m_DeedTypeFlagsOffset) = static_cast<std::uint8_t>(enabled);
    }
}