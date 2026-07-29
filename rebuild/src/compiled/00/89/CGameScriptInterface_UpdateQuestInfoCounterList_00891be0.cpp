namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void UpdateCounterList(
        long questId,
        long value,
        long maximum);
};
}

struct CGSIUpdateQuestInfoCounterList_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual void UpdateQuestInfoCounterList(
        long questId,
        long value,
        long maximum) const;
};

extern CGSIUpdateQuestInfoCounterList_GuiOwner*
    CGSIUpdateQuestInfoCounterList_Gui;

void CGameScriptInterface::UpdateQuestInfoCounterList(
    long questId,
    long value,
    long maximum) const
{
    CGSIUpdateQuestInfoCounterList_Gui->m_QuestInfo
        ->UpdateCounterList(questId, value, maximum);
}
