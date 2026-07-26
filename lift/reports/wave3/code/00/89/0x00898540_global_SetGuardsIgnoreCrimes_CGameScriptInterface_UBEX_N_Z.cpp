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
    void __thiscall SetGuardsIgnoreCrimes(bool enabled) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_SetGuardsIgnoreCrimes_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_SetGuardsIgnoreCrimes_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_SetGuardsIgnoreCrimes_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_SetGuardsIgnoreCrimes_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_SetGuardsIgnoreCrimes_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_SetGuardsIgnoreCrimes_TargetOverlay
{
    std::byte m_Pad00[0x28];
    std::uint32_t m_Flags28; // 0x28
    std::byte m_Pad2C[0x18];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_SetGuardsIgnoreCrimes_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_SetGuardsIgnoreCrimes_TargetOverlay, m_Flags28) == 0x28);
static_assert(
    offsetof(
        CGameScriptInterface_SetGuardsIgnoreCrimes_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_SetGuardsIgnoreCrimes_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_SetGuardsIgnoreCrimes_TargetOverlay, m_Flags91) == 0x91);

struct CGameScriptInterface_SetGuardsIgnoreCrimes_InterfaceOverlay
{
    std::byte m_Pad00[0x2D];
    std::uint8_t m_Field2D; // 0x2D
};
static_assert(
    offsetof(CGameScriptInterface_SetGuardsIgnoreCrimes_InterfaceOverlay, m_Field2D) == 0x2D);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_SetGuardsIgnoreCrimes_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
}

void __thiscall CGameScriptInterface::SetGuardsIgnoreCrimes(bool enabled) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_SetGuardsIgnoreCrimes_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags28 & 0x00020000) != 0))
    {
        std::int32_t interfaceTypeValue = 0x51;

        auto* pEntry =
            reinterpret_cast<CGameScriptInterface_SetGuardsIgnoreCrimes_TCInterfaceEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                    reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

        if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x51 < pEntry->m_Key))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        reinterpret_cast<CGameScriptInterface_SetGuardsIgnoreCrimes_InterfaceOverlay*>(
            pEntry->m_pInterface)
            ->m_Field2D = enabled;
    }
}