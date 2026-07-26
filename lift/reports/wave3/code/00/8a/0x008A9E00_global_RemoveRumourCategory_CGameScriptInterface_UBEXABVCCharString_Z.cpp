#include <cstddef>

class CCharString
{
public:
    CCharString(const CCharString& other);
    ~CCharString();
};

static_assert(sizeof(CCharString) == 0x04);

class CGameScriptInterface
{
public:
    void __thiscall RemoveRumourCategory(const CCharString& category) const;
};

namespace
{
struct RumourCategoryMapOverlay
{
    std::byte m_Opaque;
};

static_assert(sizeof(RumourCategoryMapOverlay) == 0x01);

using RumourCategoryMap_RemoveFn =
    void(__thiscall*)(RumourCategoryMapOverlay* pThis, const CCharString* pCategory);

inline auto* const RumourCategoryMap__Remove =
    reinterpret_cast<RumourCategoryMap_RemoveFn>(0x008AE420);

inline auto* const DAT_013BAE44 =
    reinterpret_cast<RumourCategoryMapOverlay*>(0x013BAE44);
}

void __thiscall CGameScriptInterface::RemoveRumourCategory(
    const CCharString& category) const
{
    CCharString categoryCopy(category);
    RumourCategoryMap__Remove(DAT_013BAE44, &categoryCopy);
}