namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void RemoveElement(long elementId);
};
}

struct CGSIRemoveQuestInfoElement_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual void RemoveQuestInfoElement(long elementId) const;
};

extern CGSIRemoveQuestInfoElement_GuiOwner*
    CGSIRemoveQuestInfoElement_Gui;

void CGameScriptInterface::RemoveQuestInfoElement(
    long elementId) const
{
    CGSIRemoveQuestInfoElement_Gui->m_QuestInfo
        ->RemoveElement(elementId);
}
