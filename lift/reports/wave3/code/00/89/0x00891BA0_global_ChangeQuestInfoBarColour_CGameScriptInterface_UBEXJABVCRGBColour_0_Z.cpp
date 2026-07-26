#include <cstddef>

class CRGBColour;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void __thiscall ChangeBarColour(
        long questID,
        const CRGBColour& primaryColour,
        const CRGBColour& secondaryColour);
};
}

struct CDrawQuestInfoOwnerOverlay
{
    std::byte pad_0[0x1C8];
    NPlayerGui::CDrawQuestInfo* pDrawQuestInfo;
};

static_assert(offsetof(CDrawQuestInfoOwnerOverlay, pDrawQuestInfo) == 0x1C8);

extern CDrawQuestInfoOwnerOverlay* DAT_013b8790;

class CGameScriptInterface
{
public:
    virtual void __thiscall ChangeQuestInfoBarColour(
        long questID,
        const CRGBColour& primaryColour,
        const CRGBColour& secondaryColour) const;
};

void __thiscall CGameScriptInterface::ChangeQuestInfoBarColour(
    long questID,
    const CRGBColour& primaryColour,
    const CRGBColour& secondaryColour) const
{
    DAT_013b8790->pDrawQuestInfo->ChangeBarColour(
        questID,
        primaryColour,
        secondaryColour);
}