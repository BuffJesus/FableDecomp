class CCharString;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long AddTick(
        const CCharString& labelText,
        bool completed,
        float fadeTime);
};
}

struct CGSIAddQuestInfoTickText_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual long AddQuestInfoTick(
        const CCharString& labelText,
        bool completed,
        float fadeTime) const;
};

extern CGSIAddQuestInfoTickText_GuiOwner*
    CGSIAddQuestInfoTickText_Gui;

long CGameScriptInterface::AddQuestInfoTick(
    const CCharString& labelText,
    bool completed,
    float fadeTime) const
{
    return CGSIAddQuestInfoTickText_Gui->m_QuestInfo
        ->AddTick(labelText, completed, fadeTime);
}
