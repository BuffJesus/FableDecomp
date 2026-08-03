namespace
{
struct CScriptConversation
{
    void RemoveAndCleanup(long conversationId, bool doCleanup);
};
}

class CGameScriptInterface
{
public:
    void __fastcall RemoveConversation(void* /*edx*/, long conversationId, bool doCleanup) const;
};

namespace
{
struct CGameScriptInterface_RemoveConversation_WorldOverlay
{
    unsigned char m_Pad00[0x7C];
    CScriptConversation* m_Field7C; // 0x7C
};

struct CGameScriptInterface_RemoveConversation_SelfOverlay
{
    unsigned char m_Pad00[0x04];
    CGameScriptInterface_RemoveConversation_WorldOverlay* m_pWorld; // 0x04
};
}

void __fastcall CGameScriptInterface::RemoveConversation(
    void* /*edx*/,
    long conversationId,
    bool doCleanup) const
{
    const CGameScriptInterface_RemoveConversation_SelfOverlay* const self =
        reinterpret_cast<const CGameScriptInterface_RemoveConversation_SelfOverlay*>(this);
    self->m_pWorld->m_Field7C->RemoveAndCleanup(conversationId, doCleanup);
}