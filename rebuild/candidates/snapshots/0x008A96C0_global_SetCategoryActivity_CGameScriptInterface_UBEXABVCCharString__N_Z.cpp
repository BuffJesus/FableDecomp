#include <cstddef>
#include <map>

struct CCharString
{
    std::byte m_Data[0x04];
};

static_assert(sizeof(CCharString) == 0x04);

extern std::map<CCharString, bool> DAT_013bae2c;

class CGameScriptInterface
{
public:
    virtual void __thiscall SetCategoryActivity(const CCharString& category, bool active) const;
};

namespace
{
using CCharString_CopyConstructFn =
    void(__thiscall*)(CCharString* pThis, const CCharString* pOther);

using CCharString_DestructFn =
    void(__thiscall*)(CCharString* pThis);

inline constexpr auto CCharString__CCharString =
    reinterpret_cast<CCharString_CopyConstructFn>(0x0099EC30);

inline constexpr auto CCharString___dtor =
    reinterpret_cast<CCharString_DestructFn>(0x0099EADC);
}

void __thiscall CGameScriptInterface::SetCategoryActivity(const CCharString& category, bool active) const
{
    alignas(CCharString) std::byte categoryStorage[sizeof(CCharString)];
    auto* const categoryCopy = reinterpret_cast<CCharString*>(categoryStorage);

    CCharString__CCharString(categoryCopy, &category);
    DAT_013bae2c[*categoryCopy] = active;
    CCharString___dtor(categoryCopy);
}