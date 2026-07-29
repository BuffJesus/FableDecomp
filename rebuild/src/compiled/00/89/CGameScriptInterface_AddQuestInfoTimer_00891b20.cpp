class CTimer;
class CCharString;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long AddCounter(
        const CTimer& timer,
        const CCharString& labelText,
        float fadeTime);
};
}

struct CGSIAddQuestInfoTimer_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual long AddQuestInfoTimer(
        const CTimer& timer,
        const CCharString& labelText,
        float fadeTime) const;
};

extern CGSIAddQuestInfoTimer_GuiOwner*
    CGSIAddQuestInfoTimer_Gui;

long CGameScriptInterface::AddQuestInfoTimer(
    const CTimer& timer,
    const CCharString& labelText,
    float fadeTime) const
{
    return CGSIAddQuestInfoTimer_Gui->m_QuestInfo
        ->AddCounter(timer, labelText, fadeTime);
}
