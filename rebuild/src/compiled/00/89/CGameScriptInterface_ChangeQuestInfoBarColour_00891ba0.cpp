class CRGBColour;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void ChangeBarColour(
        long questId,
        const CRGBColour& primaryColour,
        const CRGBColour& secondaryColour);
};
}

struct CGSIChangeQuestInfoBarColour_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual void ChangeQuestInfoBarColour(
        long questId,
        const CRGBColour& primaryColour,
        const CRGBColour& secondaryColour) const;
};

extern CGSIChangeQuestInfoBarColour_GuiOwner*
    CGSIChangeQuestInfoBarColour_Gui;

void CGameScriptInterface::ChangeQuestInfoBarColour(
    long questId,
    const CRGBColour& primaryColour,
    const CRGBColour& secondaryColour) const
{
    CGSIChangeQuestInfoBarColour_Gui->m_QuestInfo
        ->ChangeBarColour(
            questId,
            primaryColour,
            secondaryColour);
}
