namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void UpdateTimer(long questId, float timeValue);
};
}

struct CGSIUpdateQuestInfoTimer_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual void UpdateQuestInfoTimer(
        long questId,
        float timeValue) const;
};

extern CGSIUpdateQuestInfoTimer_GuiOwner*
    CGSIUpdateQuestInfoTimer_Gui;

__declspec(naked) void CGameScriptInterface::UpdateQuestInfoTimer(
    long questId,
    float timeValue) const
{
    __asm
    {
        mov edx, dword ptr [CGSIUpdateQuestInfoTimer_Gui]
        mov ecx, dword ptr [edx + 1C8h]
        jmp NPlayerGui::CDrawQuestInfo::UpdateTimer
        int 3
        int 3
        int 3
    }
}
