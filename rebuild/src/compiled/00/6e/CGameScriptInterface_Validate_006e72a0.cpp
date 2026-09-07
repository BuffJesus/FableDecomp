// Retail 0x006E72A0. Reads the object held at +4, calls its virtual slot 3
// (vftable offset 0x0C) to obtain a CNavigatorManager, then forwards to
// CNavigatorManager::InterruptAllNavigators (0x00A36FE0).
struct CNavigatorManager {
    void InterruptAllNavigators();
};

struct CGameScriptWorldBase {
    virtual void* VSlot0() = 0;
    virtual void* VSlot1() = 0;
    virtual void* VSlot2() = 0;
    virtual CNavigatorManager* GetNavigatorManager() = 0;
};

struct CGameScriptInterface {
    void* m_pUnknown0;
    CGameScriptWorldBase* m_pWorld;
    void Validate();
};

void CGameScriptInterface::Validate() {
    m_pWorld->GetNavigatorManager()->InterruptAllNavigators();
}