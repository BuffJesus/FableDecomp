#include <stdio.h>
static int g_interrupted = 0;

struct CNavigatorManager {
    void InterruptAllNavigators();
};
void CNavigatorManager::InterruptAllNavigators() { g_interrupted = 1; }

struct CGameScriptWorldBase {
    virtual void* VSlot0() = 0;
    virtual void* VSlot1() = 0;
    virtual void* VSlot2() = 0;
    virtual CNavigatorManager* GetNavigatorManager() = 0;
};

static CNavigatorManager g_manager;
struct CTestWorld : CGameScriptWorldBase {
    void* VSlot0() { return 0; }
    void* VSlot1() { return 0; }
    void* VSlot2() { return 0; }
    CNavigatorManager* GetNavigatorManager() { return &g_manager; }
};

struct CGameScriptInterface {
    void* m_pUnknown0;
    CGameScriptWorldBase* m_pWorld;
    void Validate();
};
void CGameScriptInterface::Validate() { m_pWorld->GetNavigatorManager()->InterruptAllNavigators(); }

int main() {
    CTestWorld world;
    CGameScriptInterface iface;
    iface.m_pUnknown0 = 0;
    iface.m_pWorld = &world;
    iface.Validate();
    if (g_interrupted != 1) return 1;
    printf("VALIDATE PASS\n");
    return 0;
}