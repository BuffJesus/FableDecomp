class CCharString;
class CRGBColour;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long AddBar(
        float value,
        float maxValue,
        const CRGBColour& filledColour,
        const CRGBColour& emptyColour,
        const CCharString& labelText,
        const CCharString& valueText,
        float fadeTime);
};
}

struct CGSIAddQuestInfoBar_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual long AddQuestInfoBar(
        float value,
        float maxValue,
        const CRGBColour& filledColour,
        const CRGBColour& emptyColour,
        const CCharString& labelText,
        const CCharString& valueText,
        float fadeTime) const;
};

extern CGSIAddQuestInfoBar_GuiOwner*
    CGSIAddQuestInfoBar_Gui;

__declspec(naked) long CGameScriptInterface::AddQuestInfoBar(
    float value,
    float maxValue,
    const CRGBColour& filledColour,
    const CRGBColour& emptyColour,
    const CCharString& labelText,
    const CCharString& valueText,
    float fadeTime) const
{
    __asm
    {
        mov eax, dword ptr [CGSIAddQuestInfoBar_Gui]
        mov ecx, dword ptr [eax + 1C8h]
        jmp NPlayerGui::CDrawQuestInfo::AddBar
    }
}
