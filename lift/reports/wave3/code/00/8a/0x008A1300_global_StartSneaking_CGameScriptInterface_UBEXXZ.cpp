#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCStealth;

enum ETCInterfaceType : int;

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

class CTCStealth
{
public:
    void __thiscall StartSneaking();
};

class CGameScriptInterface
{
public:
    void __thiscall StartSneaking() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_StartSneaking_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(
        CGameScriptInterface_StartSneaking_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_StartSneaking_TCInterfaceEntryOverlay
{
    std::int32_t m_Key; // 0x00
    CTCBase* m_pValue;  // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_StartSneaking_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_StartSneaking_TCInterfaceEntryOverlay,
        m_pValue) == 0x04);

struct CGameScriptInterface_StartSneaking_TargetOverlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24; // 0x24
    std::byte m_Pad28[0x1C];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_StartSneaking_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(
        CGameScriptInterface_StartSneaking_TargetOverlay,
        m_Flags24) == 0x24);
static_assert(
    offsetof(
        CGameScriptInterface_StartSneaking_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_StartSneaking_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_StartSneaking_TargetOverlay,
        m_Flags91) == 0x91);

using CGameScriptInterface_StartSneaking_Helper00449970Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_StartSneaking_Helper00487DE0Fn =
    CGameScriptInterface_StartSneaking_TargetOverlay* (__thiscall*)(void*);

auto* const CGameScriptInterface_StartSneaking_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_StartSneaking_Helper00449970Fn>(0x00449970);
auto* const CGameScriptInterface_StartSneaking_Helper00487DE0 =
    reinterpret_cast<CGameScriptInterface_StartSneaking_Helper00487DE0Fn>(0x00487DE0);
}

void __thiscall CGameScriptInterface::StartSneaking() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_StartSneaking_SelfOverlay*>(this);

    auto* const target =
        CGameScriptInterface_StartSneaking_Helper00487DE0(
            CGameScriptInterface_StartSneaking_Helper00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags24 & 0x00400000) != 0)
        {
            std::int32_t interfaceTypeValue = 0x36;

            auto* pEntry =
                reinterpret_cast<CGameScriptInterface_StartSneaking_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x36 < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            reinterpret_cast<CTCStealth*>(pEntry->m_pValue)->StartSneaking();
        }
    }
}