#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    void* __thiscall LowerBound(const TKey* pKey);
};

class CGameScriptInterface
{
public:
    void __thiscall SetWhetherToFailQuestOnDeath(bool shouldFailOnDeath) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_SetWhetherToFailQuestOnDeath_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(
        CGameScriptInterface_SetWhetherToFailQuestOnDeath_SelfOverlay,
        m_Field14) == 0x14);

struct TCInterfaceEntryOverlay
{
    std::int32_t m_Key; // 0x00
    CTCBase* m_pValue; // 0x04
};
static_assert(offsetof(TCInterfaceEntryOverlay, m_Key) == 0x00);
static_assert(offsetof(TCInterfaceEntryOverlay, m_pValue) == 0x04);

struct TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(TargetOverlay, m_Flags20) == 0x20);
static_assert(offsetof(TargetOverlay, m_TCInterfaceMapStorage) == 0x44);
static_assert(offsetof(TargetOverlay, m_pTCInterfaceMapEnd) == 0x48);
static_assert(offsetof(TargetOverlay, m_Flags91) == 0x91);

struct CTCInterfaceFlagOverlay
{
    std::byte m_Pad00[0x174];
    bool m_Field174; // 0x174
};
static_assert(offsetof(CTCInterfaceFlagOverlay, m_Field174) == 0x174);

using Helper00449970Fn = void* (__thiscall*)(void*);
using Helper00487DC0Fn = TargetOverlay* (__thiscall*)(void*);

auto* const Helper00449970 =
    reinterpret_cast<Helper00449970Fn>(0x00449970);
auto* const Helper00487DC0 =
    reinterpret_cast<Helper00487DC0Fn>(0x00487DC0);
}

void __thiscall CGameScriptInterface::SetWhetherToFailQuestOnDeath(
    bool shouldFailOnDeath) const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_SetWhetherToFailQuestOnDeath_SelfOverlay*>(this);

    auto* const target =
        Helper00487DC0(Helper00449970(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags20 & 0x10) != 0))
    {
        std::int32_t interfaceTypeValue = 4;

        auto* entry =
            reinterpret_cast<TCInterfaceEntryOverlay*>(
                reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage)
                    ->LowerBound(
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

        if ((entry == target->m_pTCInterfaceMapEnd) || (4 < entry->m_Key))
        {
            entry = target->m_pTCInterfaceMapEnd;
        }

        reinterpret_cast<CTCInterfaceFlagOverlay*>(entry->m_pValue)->m_Field174 =
            shouldFailOnDeath;
    }
}