#include <cstdio>

struct CAInputProcess;

struct CInputProcessManager {
    int registered;
    CAInputProcess* last;
    void Register(CAInputProcess* p);
};

struct CAInputProcess {
    CInputProcessManager* manager;
    int onAddedCalled;
    virtual void v0() {}
    virtual void v1() {}
    virtual void OnAdded() { onAddedCalled = 1; }
    virtual bool AddToInterface(CInputProcessManager* mgr);
};

void CInputProcessManager::Register(CAInputProcess* p) {
    registered++;
    last = p;
}

bool CAInputProcess::AddToInterface(CInputProcessManager* mgr)
{
    this->manager = mgr;
    mgr->Register(this);
    this->OnAdded();
    return true;
}

int main() {
    CInputProcessManager mgr; mgr.registered = 0; mgr.last = 0;
    CAInputProcess p; p.manager = 0; p.onAddedCalled = 0;
    bool r = p.AddToInterface(&mgr);
    if (r && p.manager == &mgr && mgr.registered == 1 && mgr.last == &p && p.onAddedCalled == 1) {
        printf("ADDTOINTERFACE_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}