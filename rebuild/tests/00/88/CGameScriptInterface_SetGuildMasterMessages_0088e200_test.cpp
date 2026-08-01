#include <cstdio>

struct CGameScriptInterfaceOwnerOverlay
{
    unsigned char m_Pad00[0x4];
    void* m_pOwner;
};

struct CGameScriptInterfaceGuildMasterMessagesOwnerOverlay
{
    unsigned char m_Pad00[0xE0];
    unsigned char m_bGuildMasterMessages;
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

int main()
{
    CGameScriptInterfaceGuildMasterMessagesOwnerOverlay ownerObj;
    ownerObj.m_bGuildMasterMessages = 0;

    CGameScriptInterfaceOwnerOverlay holder;
    holder.m_pOwner = &ownerObj;

    CGameScriptInterface* iface = reinterpret_cast<CGameScriptInterface*>(&holder);
    iface->CGameScriptInterface::SetGuildMasterMessages(true);
    if (ownerObj.m_bGuildMasterMessages != 1) { printf("FAIL set true\n"); return 1; }

    iface->CGameScriptInterface::SetGuildMasterMessages(false);
    if (ownerObj.m_bGuildMasterMessages != 0) { printf("FAIL set false\n"); return 1; }

    printf("GUILDMASTER_OK\n");
    return 0;
}