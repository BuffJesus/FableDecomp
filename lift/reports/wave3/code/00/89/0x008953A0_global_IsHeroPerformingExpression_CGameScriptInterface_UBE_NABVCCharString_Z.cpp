#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;

struct CCharStringRep
{
    const char* m_pString; // 0x00
    std::int32_t m_Length; // 0x04
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
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

class CGameScriptInterface
{
public:
    bool __thiscall IsHeroPerformingExpression(const CCharString& expression) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_IsHeroPerformingExpression_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(
    offsetof(CGameScriptInterface_IsHeroPerformingExpression_SelfOverlay, m_Field14) == 0x14);

struct HeroThingOverlay
{
    std::byte m_Pad00[0x30];
    std::int32_t m_Field30;      // 0x30
    std::byte m_Pad34[0x10];
    TCInterfaceMap m_Map44;      // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91;      // 0x91
};

static_assert(offsetof(HeroThingOverlay, m_Field30) == 0x30);
static_assert(offsetof(HeroThingOverlay, m_Map44) == 0x44);
static_assert(offsetof(HeroThingOverlay, m_Flags91) == 0x91);

struct ExpressionInterfaceOverlay
{
    std::byte m_Pad00[0x0C];
    void* m_Field0C; // 0x0C
};

static_assert(offsetof(ExpressionInterfaceOverlay, m_Field0C) == 0x0C);

using Helper00449970Fn = void* (__thiscall*)(void*);
using Helper00487DC1Fn = HeroThingOverlay* (__thiscall*)(void*);
using RawExpressionQueryFn =
    void* (__thiscall*)(void* pThis, void* pIteratorStorage, const CCharString* pExpression);
using RawIteratorBase12GetcontFn = std::uint8_t (__thiscall*)(void*);

inline constexpr auto Helper00449970 =
    reinterpret_cast<Helper00449970Fn>(0x00449970);
inline constexpr auto Helper00487DC1 =
    reinterpret_cast<Helper00487DC1Fn>(0x00487DC1);
inline constexpr auto RawIteratorBase12Getcont =
    reinterpret_cast<RawIteratorBase12GetcontFn>(0x006AEB10);
}

bool __thiscall CGameScriptInterface::IsHeroPerformingExpression(
    const CCharString& expression) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_IsHeroPerformingExpression_SelfOverlay*>(this);

    auto* const hero = Helper00487DC1(Helper00449970(self->m_Field14));
    if ((hero == nullptr) ||
        ((hero->m_Flags91 & 0x01) != 0) ||
        !(static_cast<std::int8_t>(static_cast<std::uint32_t>(hero->m_Field30) >> 8) < 0))
    {
        return false;
    }

    ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x8F);
    CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
        hero->m_Map44.LowerBound(&interfaceType);

    if ((pEntry == hero->m_Map44.m_pEnd) ||
        (static_cast<ETCInterfaceType>(0x8F) < pEntry->m_Key))
    {
        pEntry = hero->m_Map44.m_pEnd;
    }

    if ((expression.m_pRep != nullptr) && (expression.m_pRep->m_Length != 0))
    {
        void* iteratorStorage = nullptr;

        auto* const pInterface =
            reinterpret_cast<ExpressionInterfaceOverlay*>(pEntry->m_Value);
        auto* const pDispatchObject = pInterface->m_Field0C;

        const auto rawQuery =
            reinterpret_cast<RawExpressionQueryFn>(
                (*reinterpret_cast<void***>(pDispatchObject))[0x28 / sizeof(void*)]);

        void* const pIterator =
            rawQuery(pDispatchObject, &iteratorStorage, &expression);

        if (RawIteratorBase12Getcont(pIterator) == 0)
        {
            return false;
        }
    }

    return true;
}