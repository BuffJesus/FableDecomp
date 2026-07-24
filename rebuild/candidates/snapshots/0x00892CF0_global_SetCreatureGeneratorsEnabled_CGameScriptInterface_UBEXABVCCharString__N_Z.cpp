#include <cstddef>

template <typename T>
class CBasicString
{
public:
    bool __thiscall operator==(const T* rhs) const;
};

struct CCharString
{
    CBasicString<char>* m_pString; // 0x00
};

static_assert(sizeof(CCharString) == 0x04);
static_assert(offsetof(CCharString, m_pString) == 0x00);

class CWorld;
class CWorldMap;
class CCategory;

namespace NInventory
{
class CTCInventoryBase
{
public:
    CCategory* __thiscall GetCategoryWithIndex(long index);
};
}

class CWorldMap
{
public:
    long __thiscall GetRegionNumberFromName(const CCharString* regionName);
};

class CGameScriptInterface
{
public:
    virtual void __thiscall SetCreatureGeneratorsEnabled(
        const CCharString& regionName,
        bool enabled) const;
};

namespace
{
struct CWorldVTableOverlay
{
    std::byte m_Pad00[0x30];
    NInventory::CTCInventoryBase* (__thiscall* m_GetTCInventoryBase)(CWorld*); // 0x30
    CWorldMap* (__thiscall* m_GetWorldMap)(CWorld*);                            // 0x34
};

static_assert(offsetof(CWorldVTableOverlay, m_GetTCInventoryBase) == 0x30);
static_assert(offsetof(CWorldVTableOverlay, m_GetWorldMap) == 0x34);

struct CWorldOverlay
{
    CWorldVTableOverlay* m_pVTable; // 0x00
    std::byte m_Pad04[0xD0];
    bool m_CreatureGeneratorsEnabled; // 0xD4
};

static_assert(offsetof(CWorldOverlay, m_pVTable) == 0x00);
static_assert(offsetof(CWorldOverlay, m_CreatureGeneratorsEnabled) == 0xD4);

struct CGameScriptInterfaceOverlay
{
    std::byte m_Pad00[0x04];
    CWorld* m_pWorld; // 0x04
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_pWorld) == 0x04);

struct CCategoryEnabledOverlay
{
    std::byte m_Pad00[0x55];
    bool m_Enabled; // 0x55
};

static_assert(offsetof(CCategoryEnabledOverlay, m_Enabled) == 0x55);
}

void __thiscall CGameScriptInterface::SetCreatureGeneratorsEnabled(
    const CCharString& regionName,
    bool enabled) const
{
    const auto* const pThis = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);

    if (regionName.m_pString == nullptr)
    {
        int count = 1;
        bool equal = true;
        const char* lhs = "";
        const char* rhs = "";

        do
        {
            if (count == 0)
            {
                break;
            }

            --count;
            equal = (*lhs == *rhs);
            ++lhs;
            ++rhs;
        } while (equal);

        if (equal)
        {
SetWorldFlag:
            reinterpret_cast<CWorldOverlay*>(pThis->m_pWorld)->m_CreatureGeneratorsEnabled = enabled;
            return;
        }
    }
    else
    {
        const bool equal = regionName.m_pString->operator==("");
        if (equal)
        {
            goto SetWorldFlag;
        }
    }

    CWorld* const pWorld = pThis->m_pWorld;
    CWorldMap* const pWorldMap =
        reinterpret_cast<CWorldOverlay*>(pWorld)->m_pVTable->m_GetWorldMap(pWorld);
    const long regionNumber = pWorldMap->GetRegionNumberFromName(&regionName);
    if (regionNumber > 0)
    {
        NInventory::CTCInventoryBase* const pInventory =
            reinterpret_cast<CWorldOverlay*>(pWorld)->m_pVTable->m_GetTCInventoryBase(pWorld);
        CCategory* const pCategory = pInventory->GetCategoryWithIndex(regionNumber);
        reinterpret_cast<CCategoryEnabledOverlay*>(pCategory)->m_Enabled = enabled;
    }
}