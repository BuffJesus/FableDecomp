namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void UpdateBar(
        long questId,
        float value,
        float minimum,
        float maximum);
};
}

struct CGSIUpdateQuestInfoBar_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual void UpdateQuestInfoBar(
        long questId,
        float value,
        float minimum,
        float maximum) const;
};

extern CGSIUpdateQuestInfoBar_GuiOwner*
    CGSIUpdateQuestInfoBar_Gui;

void CGameScriptInterface::UpdateQuestInfoBar(
    long questId,
    float value,
    float minimum,
    float maximum) const
{
    CGSIUpdateQuestInfoBar_Gui->m_QuestInfo
        ->UpdateBar(questId, value, minimum, maximum);
}
