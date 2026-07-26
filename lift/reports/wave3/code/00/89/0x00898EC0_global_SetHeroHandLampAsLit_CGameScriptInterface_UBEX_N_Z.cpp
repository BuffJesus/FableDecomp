#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCLight;

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

class CTCLight
{
public:
    void __thiscall SetActive(std::uint32_t isActiveRaw);
};

class CGameScriptInterface
{
public:
    void __thiscall SetHeroHandLampAsLit(std::uint32_t rawLampValue) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_SetHeroHandLampAsLit_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroHandLampAsLit_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_SetHeroHandLampAsLit_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroHandLampAsLit_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroHandLampAsLit_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_SetHeroHandLampAsLit_TargetOverlay
{
    std::byte m_Pad00[0x24];
    std::int8_t m_Field24; // 0x24
    std::byte m_Pad25[0x1F];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_SetHeroHandLampAsLit_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroHandLampAsLit_TargetOverlay,
        m_Field24) == 0x24);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroHandLampAsLit_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroHandLampAsLit_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroHandLampAsLit_TargetOverlay,
        m_Flags91) == 0x91);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_SetHeroHandLampAsLit_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
}

void __thiscall CGameScriptInterface::SetHeroHandLampAsLit(
    std::uint32_t rawLampValue) const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_SetHeroHandLampAsLit_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if (target->m_Field24 < 0)
        {
            std::int32_t interfaceTypeValue = 0x27;

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_SetHeroHandLampAsLit_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(
                            target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) ||
                (0x27 < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            reinterpret_cast<CTCLight*>(pEntry->m_pInterface)->SetActive(rawLampValue);
            return;
        }

        reinterpret_cast<CTCLight*>(static_cast<std::uintptr_t>(rawLampValue))
            ->SetActive(rawLampValue);
    }
}