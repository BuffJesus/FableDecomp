struct CGameScriptInterfaceOwnerOverlay
{
    unsigned char m_Pad00[0x4];
    void* m_pOwner; // 0x4
};

struct CGameScriptInterfaceGuildMasterMessagesOwnerOverlay
{
    unsigned char m_Pad00[0xE0];
    unsigned char m_bGuildMasterMessages; // 0xE0
};

class CGameScriptInterface
{
public:
    virtual void SetGuildMasterMessages(bool enabled) const;
};

void CGameScriptInterface::SetGuildMasterMessages(bool enabled) const
{
    const CGameScriptInterfaceOwnerOverlay* ownerHolder = reinterpret_cast<const CGameScriptInterfaceOwnerOverlay*>(this);
    CGameScriptInterfaceGuildMasterMessagesOwnerOverlay* owner = reinterpret_cast<CGameScriptInterfaceGuildMasterMessagesOwnerOverlay*>(ownerHolder->m_pOwner);
    owner->m_bGuildMasterMessages = enabled;
}