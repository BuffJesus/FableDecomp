namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void DisplayQuestInfo(bool display);
};
}

struct CGSIDisplayQuestInfo_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual void DisplayQuestInfo(bool display) const;
};

extern CGSIDisplayQuestInfo_GuiOwner*
    CGSIDisplayQuestInfo_Gui;

void CGameScriptInterface::DisplayQuestInfo(
    bool /*display*/) const
{
    CGSIDisplayQuestInfo_Gui->m_QuestInfo
        ->DisplayQuestInfo(true);
}
