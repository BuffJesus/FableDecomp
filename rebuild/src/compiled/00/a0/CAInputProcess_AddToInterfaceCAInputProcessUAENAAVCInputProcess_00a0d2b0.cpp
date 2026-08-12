// CAInputProcess::AddToInterface  0x00a0d2b0
struct CAInputProcess;

struct CInputProcessManager {
    void Register(CAInputProcess* p);  // __fastcall, target 0xa0d4f0
};

struct CAInputProcess {
    // vtable ptr at +0
    CInputProcessManager* manager;     // +4
    virtual void v0();
    virtual void v1();
    virtual void OnAdded();            // slot 2 -> [vtbl+8]
    virtual bool AddToInterface(CInputProcessManager* mgr);
};

bool CAInputProcess::AddToInterface(CInputProcessManager* mgr)
{
    this->manager = mgr;
    mgr->Register(this);
    this->OnAdded();
    return true;
}