#include <cstddef>
#include <cstdint>

class CCharString
{
public:
    CCharString(const CCharString& other);
};

static_assert(sizeof(CCharString) == 0x04);

class CTCBase;
class CTCCreditsUI;

enum ETCInterfaceType : std::int32_t
{
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;     // 0x00
    TValue m_Value; // 0x04
};

static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Key) == 0x00);
static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Value) == 0x04);
static_assert(sizeof(CKeyPair<ETCInterfaceType, CTCBase*>) == 0x08);

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

class CTCCreditsUI
{
public:
    void __thiscall Activate(CCharString screenName);
};

class CGameScriptInterface
{
public:
    void __thiscall KickOffCreditsScreen(CCharString& screenName);
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterfaceVTableOverlay
{
    std::byte m_Pad00[0x1C];
    void (__thiscall* m_RawSlot1C)(CGameScriptInterface* pThis); // 0x1C
};

static_assert(offsetof(CGameScriptInterfaceVTableOverlay, m_RawSlot1C) == 0x1C);

struct CGameScriptInterfaceOverlay
{
    const CGameScriptInterfaceVTableOverlay* m_pVTable; // 0x00
    std::byte m_Pad04[0x10];
    void* m_Field14; // 0x14
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_Field14) == 0x14);

struct KickOffCreditsScreen_ResolvedOverlay
{
    std::byte m_Pad00[0x40];
    std::uint8_t m_Flags40;  // 0x40
    std::byte m_Pad41[0x03];
    TCInterfaceMap m_InterfaceMap44; // 0x44
};

static_assert(offsetof(KickOffCreditsScreen_ResolvedOverlay, m_Flags40) == 0x40);
static_assert(offsetof(KickOffCreditsScreen_ResolvedOverlay, m_InterfaceMap44) == 0x44);

using Helper00449970Fn = void* (__thiscall*)(void*);
using Helper00487DC0Fn = KickOffCreditsScreen_ResolvedOverlay* (__thiscall*)(void*);

inline constexpr auto Helper00449970 =
    reinterpret_cast<Helper00449970Fn>(0x00449970);
inline constexpr auto Helper00487DC0 =
    reinterpret_cast<Helper00487DC0Fn>(0x00487DC0);
}

void __thiscall CGameScriptInterface::KickOffCreditsScreen(CCharString& screenName)
{
    const auto* const self = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);

    CTCCreditsUI* pCreditsUI = nullptr;
    KickOffCreditsScreen_ResolvedOverlay* const pResolved =
        Helper00487DC0(Helper00449970(self->m_Field14));

    if ((pResolved->m_Flags40 & 0x02) != 0)
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x101);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            pResolved->m_InterfaceMap44.LowerBound(&interfaceType);

        if ((pEntry == pResolved->m_InterfaceMap44.m_pEnd) ||
            (static_cast<ETCInterfaceType>(0x101) < pEntry->m_Key))
        {
            pEntry = pResolved->m_InterfaceMap44.m_pEnd;
        }

        pCreditsUI = reinterpret_cast<CTCCreditsUI*>(pEntry->m_Value);
    }

    CCharString localScreenName(screenName);
    pCreditsUI->Activate(localScreenName);
    self->m_pVTable->m_RawSlot1C(this);
}