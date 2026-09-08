#include "engine/CAInputProcess.h"

struct CInputProcessManager { void Register(CAInputProcess* process); };
struct CAInputProcess_Methods : CAInputProcess {
    virtual void v0(); virtual void v1(); virtual void OnAdded();
    virtual bool AddToInterface(CInputProcessManager* manager);
};

bool CAInputProcess_Methods::AddToInterface(CInputProcessManager* manager) {
    CAInputProcess* process = reinterpret_cast<CAInputProcess*>(this);
    process->PPlayerInterface = manager;
    manager->Register(process);
    OnAdded();
    return true;
}
