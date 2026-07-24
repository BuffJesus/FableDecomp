#include <cstddef>
#include <cstdint>

class CDefString;
class CGameScriptInterface;
class CTCBase;
class CTCCarrying;
class CThing;

struct CCharStringRep
{
    const char* m_pString;    // 0x00
    std::int32_t m_Length;    // 0x04

    bool __thiscall operator==(const char* pText) const;
};

static_assert(offsetof(CCharStringRep, m_pString) == 0x00);
static_assert(offsetof(CCharStringRep, m_Length) == 0x04);
static_assert(sizeof(CCharStringRep) == 0x08);

class CCharString
{
public:
    CCharStringRep* m_pRep; // 0x00
};

static_assert(offsetof(CCharString, m_pRep) == 0x00);
static_assert(sizeof(CCharString) == 0x04);

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
    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

class CThing
{
public:
    CDefString* __thiscall GetDefName();
};

class CTCCarrying
{
public:
    CThing* __thiscall GetThingInPrimarySlot();
};

class CDefString
{
public:
    bool __thiscall operator==(const CCharString* pOther) const;
};

class CGameScriptInterface
{
public:
    bool __thiscall IsPlayerCarryingItemOfType(const CCharString& itemType) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_IsPlayerCarryingItemOfType_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(
    offsetof(CGameScriptInterface_IsPlayerCarryingItemOfType_SelfOverlay, m_Field14) == 0x14);

struct HeroThingOverlay
{
    std::byte m_Pad00[0x28];
    std::uint8_t m_Flags28; // 0x28
    std::byte m_Pad29[0x1B];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    CKeyPair<ETCInterfaceType, CTCBase*>* m_pMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(HeroThingOverlay, m_Flags28) == 0x28);
static_assert(offsetof(HeroThingOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(HeroThingOverlay, m_pMapEnd) == 0x48);
static_assert(offsetof(HeroThingOverlay, m_Flags91) == 0x91);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 = HeroThingOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);

constexpr ETCInterfaceType kCarryingInterfaceType =
    static_cast<ETCInterfaceType>(0x46);
}

bool __thiscall CGameScriptInterface::IsPlayerCarryingItemOfType(
    const CCharString& itemType) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_IsPlayerCarryingItemOfType_SelfOverlay*>(this);
    auto* const hero = CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((hero == nullptr) ||
        ((hero->m_Flags91 & 0x01u) != 0) ||
        ((hero->m_Flags28 & 0x40u) == 0))
    {
        return false;
    }

    auto* pEntry = hero->m_InterfaceMap44.LowerBound(&kCarryingInterfaceType);
    if ((pEntry == hero->m_pMapEnd) || (kCarryingInterfaceType < pEntry->m_Key))
    {
        pEntry = hero->m_pMapEnd;
    }

    CThing* const pPrimaryThing =
        reinterpret_cast<CTCCarrying*>(pEntry->m_Value)->GetThingInPrimarySlot();

    if (itemType.m_pRep == nullptr)
    {
        int count = 1;
        bool isEqual = true;
        const char* pLeft = "";
        const char* pRight = "";

        do
        {
            if (count == 0)
            {
                break;
            }

            --count;
            isEqual = (*pLeft == *pRight);
            ++pLeft;
            ++pRight;
        } while (isEqual);

        if (isEqual)
        {
            return pPrimaryThing != nullptr;
        }
    }
    else
    {
        if (itemType.m_pRep->operator==(""))
        {
            return pPrimaryThing != nullptr;
        }
    }

    if (pPrimaryThing != nullptr)
    {
        CDefString* const pDefName = pPrimaryThing->GetDefName();
        if (pDefName->operator==(&itemType))
        {
            return true;
        }
    }

    return false;
}