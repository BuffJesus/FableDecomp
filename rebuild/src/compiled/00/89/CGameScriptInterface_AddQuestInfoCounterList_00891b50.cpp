class CCharString;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long AddCounterList(
        const CCharString& labelText,
        long value,
        float fadeTime);
};
}

struct CGSIAddQuestInfoCounterList_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual long AddQuestInfoCounterList(
        const CCharString& labelText,
        long value,
        float fadeTime) const;
};

extern CGSIAddQuestInfoCounterList_GuiOwner*
    CGSIAddQuestInfoCounterList_Gui;

long CGameScriptInterface::AddQuestInfoCounterList(
    const CCharString& labelText,
    long value,
    float fadeTime) const
{
    return CGSIAddQuestInfoCounterList_Gui->m_QuestInfo
        ->AddCounterList(labelText, value, fadeTime);
}
