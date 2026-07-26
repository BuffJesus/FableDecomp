#include <cstddef>

struct CCharString
{
    std::byte m_Data[0x04];
};

static_assert(sizeof(CCharString) == 0x04);

class CGameScriptInterface
{
public:
    virtual void __thiscall SetActiveGossipCategories(CCharString category, bool active);
};

namespace
{
struct GossipCategoryMapOverlay
{
    std::byte m_Opaque;
};

static_assert(sizeof(GossipCategoryMapOverlay) == 0x01);

using CCharString_CopyConstructFn =
    void(__thiscall*)(CCharString* pThis, const CCharString* pOther);

using CCharString_DestroyFn =
    void(__thiscall*)(CCharString* pThis);

using GossipCategoryMap_OperatorIndexFn =
    bool&(__thiscall*)(GossipCategoryMapOverlay* pThis, const CCharString* pKey);

inline auto* const CCharString__CCharString =
    reinterpret_cast<CCharString_CopyConstructFn>(0x0099EC30);

inline auto* const CCharString__dtor =
    reinterpret_cast<CCharString_DestroyFn>(0x0099EAE0);

inline auto* const GossipCategoryMap__operator_index =
    reinterpret_cast<GossipCategoryMap_OperatorIndexFn>(0x008ADF10);

inline auto* const DAT_013BAE2C =
    reinterpret_cast<GossipCategoryMapOverlay*>(0x013BAE2C);
}

void __thiscall CGameScriptInterface::SetActiveGossipCategories(
    CCharString category,
    bool active)
{
    (void)this;

    CCharString categoryCopy;
    CCharString__CCharString(&categoryCopy, &category);

    bool* const pActiveFlag =
        &GossipCategoryMap__operator_index(DAT_013BAE2C, &categoryCopy);
    *pActiveFlag = active;

    CCharString__dtor(&categoryCopy);
    CCharString__dtor(&category);
}