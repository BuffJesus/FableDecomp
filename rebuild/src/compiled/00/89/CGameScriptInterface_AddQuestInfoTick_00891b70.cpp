enum EGameAction
{
    EGameAction_None = 0
};

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long AddTick(
        EGameAction action,
        bool completed,
        float fadeTime);
};
}

struct CGSIAddQuestInfoTickAction_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual long AddQuestInfoTick(
        EGameAction action,
        bool completed,
        float fadeTime) const;
};

extern CGSIAddQuestInfoTickAction_GuiOwner*
    CGSIAddQuestInfoTickAction_Gui;

long CGameScriptInterface::AddQuestInfoTick(
    EGameAction action,
    bool completed,
    float fadeTime) const
{
    return CGSIAddQuestInfoTickAction_Gui->m_QuestInfo
        ->AddTick(action, completed, fadeTime);
}
