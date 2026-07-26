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
    bool __thiscall IsHeroHandLampLit() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_IsHeroHandLampLit_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
    std::byte m_Pad18[0x04];
    std::uint8_t m_Field1C; // 0x1C
};
static_assert(
    offsetof(CGameScriptInterface_IsHeroHandLampLit_SelfOverlay, m_Field14) == 0x14);
static_assert(
    offsetof(CGameScriptInterface_IsHeroHandLampLit_SelfOverlay, m_Field1C) == 0x1C);

struct CGameScriptInterface_IsHeroHandLampLit_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_IsHeroHandLampLit_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_IsHeroHandLampLit_TargetOverlay
{
    std::byte m_Pad00[0x24];
    std::int8_t m_Field24; // 0x24
    std::byte m_Pad25[0x1F];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_IsHeroHandLampLit_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_IsHeroHandLampLit_TargetOverlay, m_Field24) == 0x24);
static_assert(
    offsetof(
        CGameScriptInterface_IsHeroHandLampLit_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_IsHeroHandLampLit_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_IsHeroHandLampLit_TargetOverlay, m_Flags91) == 0x91);

struct CTCBase_IsHeroHandLampLit_Overlay
{
    std::byte m_Pad00[0x1C];
    std::uint8_t m_Field1C; // 0x1C
};
static_assert(offsetof(CTCBase_IsHeroHandLampLit_Overlay, m_Field1C) == 0x1C);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DD0 =
    CGameScriptInterface_IsHeroHandLampLit_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DD0 CALL_00487DD0 =
    reinterpret_cast<Fn_00487DD0>(0x00487DD0);
}

bool __thiscall CGameScriptInterface::IsHeroHandLampLit() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_IsHeroHandLampLit_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DD0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if (target->m_Field24 < 0)
        {
            std::int32_t interfaceTypeValue = 0x27;

            auto* pEntry =
                reinterpret_cast<CGameScriptInterface_IsHeroHandLampLit_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 0x27))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            return reinterpret_cast<const CTCBase_IsHeroHandLampLit_Overlay*>(pEntry->m_pInterface)
                ->m_Field1C != 0;
        }

        return self->m_Field1C != 0;
    }

    return false;
}