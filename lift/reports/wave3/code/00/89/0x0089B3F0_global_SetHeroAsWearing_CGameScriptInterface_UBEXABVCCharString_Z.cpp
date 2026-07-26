#include <cstddef>
#include <cstdint>

class CCharString;
class CTCBase;
class CTCInventoryClothing
{
public:
    void __thiscall SetAsWearing(const CCharString& itemName);
};

class CGameScriptInterface
{
public:
    void __thiscall SetHeroAsWearing(const CCharString& itemName) const;
};

struct CGameScriptInterfaceSetHeroAsWearingOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(offsetof(CGameScriptInterfaceSetHeroAsWearingOverlay, m_Field14) == 0x14);

struct CTCInterfaceMapEntryOverlay
{
    std::int32_t m_Key; // 0x0
    CTCBase* m_Value;   // 0x4
};
static_assert(offsetof(CTCInterfaceMapEntryOverlay, m_Key) == 0x0);
static_assert(offsetof(CTCInterfaceMapEntryOverlay, m_Value) == 0x4);

struct CTCInterfaceMapOverlay
{
    CTCInterfaceMapEntryOverlay* m_Begin; // 0x0
    CTCInterfaceMapEntryOverlay* m_End;   // 0x4
};
static_assert(offsetof(CTCInterfaceMapOverlay, m_Begin) == 0x0);
static_assert(offsetof(CTCInterfaceMapOverlay, m_End) == 0x4);

struct SetHeroAsWearingTargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_Flags20; // 0x20
    std::byte m_Pad24[0x44 - 0x24];
    CTCInterfaceMapOverlay m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x91 - 0x4C];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(SetHeroAsWearingTargetOverlay, m_Flags20) == 0x20);
static_assert(offsetof(SetHeroAsWearingTargetOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(SetHeroAsWearingTargetOverlay, m_Flags91) == 0x91);

void __thiscall CGameScriptInterface::SetHeroAsWearing(const CCharString& itemName) const
{
    using ResolveField14Fn = void* (__thiscall*)(void*);
    using ResolveTargetFn = SetHeroAsWearingTargetOverlay* (__thiscall*)(void*);
    using LowerBoundFn = CTCInterfaceMapEntryOverlay* (__thiscall*)(CTCInterfaceMapOverlay*, const std::int32_t*);

    auto* const self = reinterpret_cast<const CGameScriptInterfaceSetHeroAsWearingOverlay*>(this);

    auto* const resolvedField14 =
        reinterpret_cast<ResolveField14Fn>(0x00449970)(self->m_Field14);
    auto* const target =
        reinterpret_cast<ResolveTargetFn>(0x00487DC0)(resolvedField14);

    if (target != nullptr &&
        (target->m_Flags91 & 0x01) == 0 &&
        (target->m_Flags20 & 0x00040000) != 0)
    {
        const std::int32_t interfaceType = 0x12;
        CTCInterfaceMapEntryOverlay* entry =
            reinterpret_cast<LowerBoundFn>(0x0040F020)(&target->m_InterfaceMap44, &interfaceType);

        if (entry == target->m_InterfaceMap44.m_End || entry->m_Key > 0x12)
        {
            entry = target->m_InterfaceMap44.m_End;
        }

        reinterpret_cast<CTCInventoryClothing*>(entry->m_Value)->SetAsWearing(itemName);
    }
}