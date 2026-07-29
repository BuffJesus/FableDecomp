class CCounter;
class CCharString;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long AddCounter(
        const CCounter& counter,
        const CCharString& labelText,
        float fadeTime);
};
}

struct CGSIAddQuestInfoCounter_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual long AddQuestInfoCounter(
        const CCounter& counter,
        const CCharString& labelText,
        float fadeTime) const;
};

extern CGSIAddQuestInfoCounter_GuiOwner*
    CGSIAddQuestInfoCounter_Gui;

long CGameScriptInterface::AddQuestInfoCounter(
    const CCounter& counter,
    const CCharString& labelText,
    float fadeTime) const
{
    return CGSIAddQuestInfoCounter_Gui->m_QuestInfo
        ->AddCounter(counter, labelText, fadeTime);
}
