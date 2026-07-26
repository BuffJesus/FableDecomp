#include <cstddef>
#include <cstdint>

struct CCharStringData;

class CCharString
{
public:
    CCharStringData* m_pData; // 0x00

    CCharString(const CCharString& other);
    ~CCharString();
};

static_assert(offsetof(CCharString, m_pData) == 0x00);
static_assert(sizeof(CCharString) == 0x04);

class CGameScriptInterface
{
public:
    void __thiscall AddGossipFactionToCategory(
        const CCharString& category,
        const CCharString& faction) const;
};

void __stdcall AddGossipFactionToCategory_Impl(
    CCharString category,
    CCharString faction);

void __thiscall CGameScriptInterface::AddGossipFactionToCategory(
    const CCharString& category,
    const CCharString& faction) const
{
    AddGossipFactionToCategory_Impl(category, faction);
}
