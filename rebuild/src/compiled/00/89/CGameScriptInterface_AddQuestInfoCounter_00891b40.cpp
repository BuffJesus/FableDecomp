class CCharString;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long AddCounter(
        const CCharString& labelText,
        long value,
        float fadeTime);
};
}

struct CGSIAddQuestInfoCounterScalar_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual long AddQuestInfoCounter(
        const CCharString& labelText,
        long value,
        float fadeTime) const;
};

extern CGSIAddQuestInfoCounterScalar_GuiOwner*
    CGSIAddQuestInfoCounterScalar_Gui;

long CGameScriptInterface::AddQuestInfoCounter(
    const CCharString& labelText,
    long value,
    float fadeTime) const
{
    return CGSIAddQuestInfoCounterScalar_Gui->m_QuestInfo
        ->AddCounter(labelText, value, fadeTime);
}
