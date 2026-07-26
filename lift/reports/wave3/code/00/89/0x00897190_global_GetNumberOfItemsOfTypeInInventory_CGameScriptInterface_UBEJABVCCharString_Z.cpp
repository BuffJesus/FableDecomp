#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CCharString;
class CDefinitionManager;
class CTCBase;

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
    CKeyPair<TKey, TValue>* LowerBound(const TKey* pKey);

    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04
};

static_assert(offsetof(CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>, m_pBegin) == 0x00);
static_assert(offsetof(CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>, m_pEnd) == 0x04);
static_assert(sizeof(CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>) == 0x08);

class CDefinitionManager
{
public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;
};

namespace NInventory
{
class CTCInventoryBase
{
public:
    long GetNumberOfItemsOfTypeInInventory(long definitionIndex) const;
};
}

struct CGameScriptInterface_Overlay
{
    std::byte m_Pad00[0x10];
    CDefinitionManager* m_pDefinitionManager; // 0x10
    void* m_Field14;                          // 0x14
};

static_assert(offsetof(CGameScriptInterface_Overlay, m_pDefinitionManager) == 0x10);
static_assert(offsetof(CGameScriptInterface_Overlay, m_Field14) == 0x14);

struct CScriptEntityInventoryOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_Flags20; // 0x20
    std::byte m_Pad24[0x20];
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>> m_Interfaces; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CScriptEntityInventoryOverlay, m_Flags20) == 0x20);
static_assert(offsetof(CScriptEntityInventoryOverlay, m_Interfaces) == 0x44);
static_assert(offsetof(CScriptEntityInventoryOverlay, m_Flags91) == 0x91);

extern "C" void* __thiscall sub_00449970(void* thisPtr);
extern "C" CScriptEntityInventoryOverlay* __thiscall sub_00489DD0(void* thisPtr);

long CGameScriptInterface::GetNumberOfItemsOfTypeInInventory(const CCharString& itemTypeName) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);

    const long definitionIndex =
        self->m_pDefinitionManager->GetDefGlobalIndexFromName(itemTypeName);
    if (definitionIndex <= 0)
    {
        return 0;
    }

    CScriptEntityInventoryOverlay* const entity =
        sub_00489DD0(sub_00449970(self->m_Field14));
    if (entity == nullptr)
    {
        return 0;
    }

    if ((entity->m_Flags91 & 0x01u) != 0)
    {
        return 0;
    }

    if ((entity->m_Flags20 & 0x00020000u) != 0)
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x11);
        CKeyPair<ETCInterfaceType, CTCBase*>* interfaceIt =
            entity->m_Interfaces.LowerBound(&interfaceType);

        if (interfaceIt == entity->m_Interfaces.m_pEnd || static_cast<std::int32_t>(interfaceType) < static_cast<std::int32_t>(interfaceIt->m_Key))
        {
            interfaceIt = entity->m_Interfaces.m_pEnd;
        }

        return reinterpret_cast<const NInventory::CTCInventoryBase*>(interfaceIt->m_Value)
            ->GetNumberOfItemsOfTypeInInventory(definitionIndex);
    }

    return reinterpret_cast<const NInventory::CTCInventoryBase*>(&itemTypeName)
        ->GetNumberOfItemsOfTypeInInventory(definitionIndex);
}