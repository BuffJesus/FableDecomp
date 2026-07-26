#include <cstddef>
#include <cstdint>

class CCharString
{
public:
    std::uint32_t value;
    CCharString(const CCharString& other);
};

static_assert(sizeof(CCharString) == 4);

extern void __cdecl CGameScriptInterface_FindOrCreateRumourCategory();

class CGameScriptInterface
{
public:
    void __thiscall AddRumourCategory(const CCharString& category) const;
};

void __thiscall CGameScriptInterface::AddRumourCategory(
    const CCharString& category) const
{
    CCharString localCategory(category);
    CGameScriptInterface_FindOrCreateRumourCategory();
}