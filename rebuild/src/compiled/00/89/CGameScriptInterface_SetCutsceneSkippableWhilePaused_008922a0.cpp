struct CGSISetCutsceneSkippablePaused_Manager
{
    unsigned char m_Pad00[0xF9];
    bool m_IsSkippableWhilePaused;
};

extern CGSISetCutsceneSkippablePaused_Manager*
    CGSISetCutsceneSkippablePaused_ManagerInstance;

class CGameScriptInterface
{
public:
    virtual void SetCutsceneSkippableWhilePaused(
        bool isSkippableWhilePaused) const;
};

void CGameScriptInterface::SetCutsceneSkippableWhilePaused(
    bool isSkippableWhilePaused) const
{
    CGSISetCutsceneSkippablePaused_ManagerInstance
        ->m_IsSkippableWhilePaused = isSkippableWhilePaused;
}
