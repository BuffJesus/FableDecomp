#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CCharString;
class CTCBase;
class CTCVillage;

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

    CKeyPair<TKey, TValue>* LowerBound(const TKey* pKey);
};

class CGameScriptInterface
{
public:
    void EnableVillagerDefTypes(const CScriptThing& thing, bool enable, const CCharString& definitionType) const;
};

class CTCVillage
{
public:
    void EnableVillagerDefTypes(bool enable, const CCharString& definitionType);
};

struct SVillagerDefTypesTarget
{
    std::byte m_Pad00[0x24];
    std::uint8_t m_Flags24; // 0x24
    std::byte m_Pad25[0x1F];
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>> m_InterfaceMap; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(SVillagerDefTypesTarget, m_Flags24) == 0x24);
static_assert(offsetof(SVillagerDefTypesTarget, m_InterfaceMap) == 0x44);
static_assert(offsetof(SVillagerDefTypesTarget, m_Flags91) == 0x91);

void CGameScriptInterface::EnableVillagerDefTypes(
    const CScriptThing& thing,
    bool enable,
    const CCharString& definitionType) const
{
    const auto vftable = *reinterpret_cast<void* const* const*>(&thing);
    const auto getTarget =
        reinterpret_cast<SVillagerDefTypesTarget* (__thiscall*)(const CScriptThing*)>(
            vftable[0x2C / sizeof(void*)]);

    SVillagerDefTypesTarget* const target = getTarget(&thing);
    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags24 & 0x04) != 0))
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x22);
        CKeyPair<ETCInterfaceType, CTCBase*>* interfaceEntry =
            target->m_InterfaceMap.LowerBound(&interfaceType);

        if ((interfaceEntry == target->m_InterfaceMap.m_pEnd) ||
            (static_cast<std::int32_t>(interfaceType) <
             static_cast<std::int32_t>(interfaceEntry->m_Key)))
        {
            interfaceEntry = target->m_InterfaceMap.m_pEnd;
        }

        static_cast<CTCVillage*>(interfaceEntry->m_Value)->EnableVillagerDefTypes(
            enable,
            definitionType);
    }
}