#include <cstdio>

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
    void* m_Field7C;
};

struct CGameScriptInterface_RemoveConversation_SelfOverlay
{
    unsigned char m_Pad00[0x04];
    CGameScriptInterface_RemoveConversation_WorldOverlay* m_pWorld;
};
}

int main()
{
    CGameScriptInterface_RemoveConversation_WorldOverlay world;
    void* sentinelField7C = reinterpret_cast<void*>(0x12345678);
    world.m_Field7C = sentinelField7C;

    CGameScriptInterface_RemoveConversation_SelfOverlay selfObj;
    selfObj.m_pWorld = &world;

    CGameScriptInterface* iface = reinterpret_cast<CGameScriptInterface*>(&selfObj);

    const CGameScriptInterface_RemoveConversation_SelfOverlay* const self =
        reinterpret_cast<const CGameScriptInterface_RemoveConversation_SelfOverlay*>(iface);

    if (self->m_pWorld->m_Field7C == sentinelField7C)
    {
        printf("RemoveConversation_PASS_FIELD7C\n");
    }
    else
    {
        printf("RemoveConversation_FAIL_FIELD7C\n");
    }

    return 0;
}