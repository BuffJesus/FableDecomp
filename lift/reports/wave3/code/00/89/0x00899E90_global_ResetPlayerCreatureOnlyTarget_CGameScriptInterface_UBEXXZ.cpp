#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CThing;
class CVillageTaskCarryCrate;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    void* __thiscall LowerBound(const TKey* pKey);
};

class CVillageTaskCarryCrate
{
public:
    void __thiscall AssignCreatureToTask(CThing* pThing);
};

class CGameScriptInterface
{
public:
    void __thiscall ResetPlayerCreatureOnlyTarget() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_ResetPlayerCreatureOnlyTarget_SelfOverlay
{
    std::byte m_Pad00[0x08];
    void* m_Field08; // 0x08
};
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureOnlyTarget_SelfOverlay,
        m_Field08) == 0x08);

struct CGameScriptInterface_ResetPlayerCreatureOnlyTarget_Field08Overlay
{
    std::byte m_Pad00[0x1C];
    void* m_Field1C; // 0x1C
};
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureOnlyTarget_Field08Overlay,
        m_Field1C) == 0x1C);

struct CGameScriptInterface_ResetPlayerCreatureOnlyTarget_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureOnlyTarget_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureOnlyTarget_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_ResetPlayerCreatureOnlyTarget_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_Flags20; // 0x20
    std::byte m_Pad24[0x20];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_ResetPlayerCreatureOnlyTarget_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureOnlyTarget_TargetOverlay,
        m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureOnlyTarget_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureOnlyTarget_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureOnlyTarget_TargetOverlay,
        m_Flags91) == 0x91);

using CGameScriptInterface_ResetPlayerCreatureOnlyTarget_Helper00449970Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_ResetPlayerCreatureOnlyTarget_Helper00487DC0Fn =
    CGameScriptInterface_ResetPlayerCreatureOnlyTarget_TargetOverlay* (__thiscall*)(void*);

inline constexpr auto CGameScriptInterface_ResetPlayerCreatureOnlyTarget_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_ResetPlayerCreatureOnlyTarget_Helper00449970Fn>(
        0x00449970);
inline constexpr auto CGameScriptInterface_ResetPlayerCreatureOnlyTarget_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_ResetPlayerCreatureOnlyTarget_Helper00487DC0Fn>(
        0x00487DC0);
}

void __thiscall CGameScriptInterface::ResetPlayerCreatureOnlyTarget() const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_ResetPlayerCreatureOnlyTarget_SelfOverlay*>(this);
    const auto* const field08 =
        reinterpret_cast<
            const CGameScriptInterface_ResetPlayerCreatureOnlyTarget_Field08Overlay*>(
            self->m_Field08);

    auto* const target =
        CGameScriptInterface_ResetPlayerCreatureOnlyTarget_Helper00487DC0(
            CGameScriptInterface_ResetPlayerCreatureOnlyTarget_Helper00449970(
                field08->m_Field1C));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags20 & 0x00000100) != 0))
    {
        std::int32_t interfaceTypeValue = 8;

        auto* pEntry =
            reinterpret_cast<
                CGameScriptInterface_ResetPlayerCreatureOnlyTarget_TCInterfaceEntryOverlay*>(
                reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage)
                    ->LowerBound(
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

        if ((pEntry == target->m_pTCInterfaceMapEnd) || (8 < pEntry->m_Key))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        reinterpret_cast<CVillageTaskCarryCrate*>(pEntry->m_pInterface)
            ->AssignCreatureToTask(nullptr);
    }
}