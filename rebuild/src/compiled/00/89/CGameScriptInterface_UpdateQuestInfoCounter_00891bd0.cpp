namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void UpdateCounter(
        long questId,
        long value,
        long maximum);
};
}

struct CGSIUpdateQuestInfoCounter_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual void UpdateQuestInfoCounter(
        long questId,
        long value,
        long maximum) const;
};

extern CGSIUpdateQuestInfoCounter_GuiOwner*
    CGSIUpdateQuestInfoCounter_Gui;

void CGameScriptInterface::UpdateQuestInfoCounter(
    long questId,
    long value,
    long maximum) const
{
    CGSIUpdateQuestInfoCounter_Gui->m_QuestInfo
        ->UpdateCounter(questId, value, maximum);
}
