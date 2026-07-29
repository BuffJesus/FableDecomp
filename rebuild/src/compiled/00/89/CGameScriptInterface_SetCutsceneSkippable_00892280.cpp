struct CGSISetCutsceneSkippable_Manager
{
    unsigned char m_Pad00[0xF8];
    bool m_IsSkippable;
};

extern CGSISetCutsceneSkippable_Manager*
    CGSISetCutsceneSkippable_ManagerInstance;

class CGameScriptInterface
{
public:
    virtual void SetCutsceneSkippable(bool isSkippable) const;
};

void CGameScriptInterface::SetCutsceneSkippable(
    bool isSkippable) const
{
    CGSISetCutsceneSkippable_ManagerInstance->m_IsSkippable =
        isSkippable;
}
