#include <cstddef>
#include <cstdint>

class CScriptThing;

enum ETCInterfaceType : std::int32_t
{
};

class CTCBase
{
public:
    std::byte m_Pad0[0xD4];
    std::uint32_t m_UnknownD4; // 0xD4
};

static_assert(offsetof(CTCBase, m_UnknownD4) == 0xD4);

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

static_assert(offsetof(CVectorMap<ETCInterfaceType, CTCBase*, void>, m_pBegin) == 0x00);
static_assert(offsetof(CVectorMap<ETCInterfaceType, CTCBase*, void>, m_pEnd) == 0x04);

class CGameScriptInterface;

class HeroInterfaceOwnerOverlay
{
public:
    std::byte m_Pad0[0x24];
    std::uint8_t m_Flags24; // 0x24
    std::byte m_Pad25[0x44 - 0x25];
    CVectorMap<ETCInterfaceType, CTCBase*, void> m_InterfaceMap; // 0x44
    std::byte m_Pad4C[0x91 - 0x4C];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(HeroInterfaceOwnerOverlay, m_Flags24) == 0x24);
static_assert(offsetof(HeroInterfaceOwnerOverlay, m_InterfaceMap) == 0x44);
static_assert(offsetof(HeroInterfaceOwnerOverlay, m_Flags91) == 0x91);

class CGameScriptInterface
{
public:
    void __thiscall ClearHeroEnemyOfGuards(const CScriptThing& thing) const
    {
        (void)thing;

        using GetTargetFn = HeroInterfaceOwnerOverlay* (__thiscall*)(const CGameScriptInterface*);
        const auto getTarget =
            reinterpret_cast<GetTargetFn>((*reinterpret_cast<void* const* const*>(this))[0x2C / sizeof(void*)]);

        HeroInterfaceOwnerOverlay* const pTarget = getTarget(this);
        if ((pTarget != nullptr) &&
            ((pTarget->m_Flags91 & 0x01) == 0) &&
            ((pTarget->m_Flags24 & 0x04) != 0))
        {
            ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x22);
            CKeyPair<ETCInterfaceType, CTCBase*>* pIt = pTarget->m_InterfaceMap.LowerBound(&interfaceType);
            if ((pIt == pTarget->m_InterfaceMap.m_pEnd) ||
                (static_cast<std::int32_t>(interfaceType) < static_cast<std::int32_t>(pIt->m_Key)))
            {
                pIt = pTarget->m_InterfaceMap.m_pEnd;
            }

            pIt->m_Value->m_UnknownD4 = 0;
        }
    }
};