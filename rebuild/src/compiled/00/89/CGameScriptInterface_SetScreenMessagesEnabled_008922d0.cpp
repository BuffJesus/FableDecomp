struct CGSISetScreenMessagesEnabled_Manager
{
    unsigned char m_Pad00[0x2BC];
    bool m_AreScreenMessagesEnabled;
};

extern CGSISetScreenMessagesEnabled_Manager*
    CGSISetScreenMessagesEnabled_ManagerInstance;

class CGameScriptInterface
{
public:
    virtual void SetScreenMessagesEnabled(bool enabled) const;
};

void CGameScriptInterface::SetScreenMessagesEnabled(
    bool enabled) const
{
    CGSISetScreenMessagesEnabled_ManagerInstance
        ->m_AreScreenMessagesEnabled = enabled;
}
