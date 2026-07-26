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
    static void* __thiscall LowerBound(
        CVectorMap* pThis,
        const TKey* pKey);
};

class CGameScriptInterface
{
public:
    bool __thiscall IsToFailQuestOnDeath() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_IsToFailQuestOnDeath_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14;
};
static_assert(
    offsetof(CGameScriptInterface_IsToFailQuestOnDeath_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_IsToFailQuestOnDeath_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;
    CTCBase* m_pInterface;
};
static_assert(
    offsetof(
        CGameScriptInterface_IsToFailQuestOnDeath_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_IsToFailQuestOnDeath_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_IsToFailQuestOnDeath_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20;
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04];
    CGameScriptInterface_IsToFailQuestOnDeath_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd;
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91;
};
static_assert(
    offsetof(CGameScriptInterface_IsToFailQuestOnDeath_TargetOverlay, m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_IsToFailQuestOnDeath_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_IsToFailQuestOnDeath_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_IsToFailQuestOnDeath_TargetOverlay, m_Flags91) == 0x91);

struct CTCInterfaceFlagOverlay
{
    std::byte m_Pad00[0x174];
    bool m_Field174;
};
static_assert(offsetof(CTCInterfaceFlagOverlay, m_Field174) == 0x174);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DD0 =
    CGameScriptInterface_IsToFailQuestOnDeath_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DD0 CALL_00487DD0 =
    reinterpret_cast<Fn_00487DD0>(0x00487DD0);
}

bool __thiscall CGameScriptInterface::IsToFailQuestOnDeath() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_IsToFailQuestOnDeath_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DD0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags20 & 0x10) != 0))
    {
        std::int32_t interfaceTypeValue = 0x04;

        auto* pEntry =
            reinterpret_cast<CGameScriptInterface_IsToFailQuestOnDeath_TCInterfaceEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                    reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

        if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x04 < pEntry->m_Key))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        return reinterpret_cast<const CTCInterfaceFlagOverlay*>(pEntry->m_pInterface)->m_Field174;
    }

    return true;
}