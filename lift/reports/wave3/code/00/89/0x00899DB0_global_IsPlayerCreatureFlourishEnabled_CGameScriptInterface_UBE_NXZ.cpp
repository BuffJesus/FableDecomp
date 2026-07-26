#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCCombat;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    void* __thiscall LowerBound(const TKey* pKey);
};

class CTCCombat
{
public:
    bool __thiscall IsMeleeFlourishValid();
};

class CGameScriptInterface
{
public:
    bool __thiscall IsPlayerCreatureFlourishEnabled() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_IsPlayerCreatureFlourishEnabled_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_IsPlayerCreatureFlourishEnabled_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_IsPlayerCreatureFlourishEnabled_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_IsPlayerCreatureFlourishEnabled_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_IsPlayerCreatureFlourishEnabled_TargetOverlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24; // 0x24
    std::byte m_Pad28[0x1C];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_IsPlayerCreatureFlourishEnabled_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(
        CGameScriptInterface_IsPlayerCreatureFlourishEnabled_TargetOverlay,
        m_Flags24) == 0x24);
static_assert(
    offsetof(
        CGameScriptInterface_IsPlayerCreatureFlourishEnabled_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_IsPlayerCreatureFlourishEnabled_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_IsPlayerCreatureFlourishEnabled_TargetOverlay,
        m_Flags91) == 0x91);

using CGameScriptInterface_IsPlayerCreatureFlourishEnabled_Helper00449970Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_IsPlayerCreatureFlourishEnabled_Helper00487DC0Fn =
    CGameScriptInterface_IsPlayerCreatureFlourishEnabled_TargetOverlay* (__thiscall*)(void*);

auto* const CGameScriptInterface_IsPlayerCreatureFlourishEnabled_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_IsPlayerCreatureFlourishEnabled_Helper00449970Fn>(
        0x00449970);
auto* const CGameScriptInterface_IsPlayerCreatureFlourishEnabled_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_IsPlayerCreatureFlourishEnabled_Helper00487DC0Fn>(
        0x00487DC0);
}

bool __thiscall CGameScriptInterface::IsPlayerCreatureFlourishEnabled() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_IsPlayerCreatureFlourishEnabled_SelfOverlay*>(
            this);
    auto* const target =
        CGameScriptInterface_IsPlayerCreatureFlourishEnabled_Helper00487DC0(
            CGameScriptInterface_IsPlayerCreatureFlourishEnabled_Helper00449970(
                self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags24 & 0x00010000) != 0))
    {
        ETCInterfaceType interfaceTypeValue = static_cast<ETCInterfaceType>(0x30);

        auto* pEntry =
            reinterpret_cast<
                CGameScriptInterface_IsPlayerCreatureFlourishEnabled_TCInterfaceEntryOverlay*>(
                reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage)
                    ->LowerBound(&interfaceTypeValue));

        if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 0x30))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        return reinterpret_cast<CTCCombat*>(pEntry->m_pInterface)->IsMeleeFlourishValid();
    }

    return false;
}