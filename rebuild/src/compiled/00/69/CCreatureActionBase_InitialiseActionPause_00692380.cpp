#include "engine/CCreatureActionBase.h"

struct ActionRegistrar { void RegisterAction(int actionId, void* description); };
extern "C" ActionRegistrar* __cdecl GetActionRegistrar();

struct CCreatureActionBaseVirtuals {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6();
    virtual int GetActionId();
};

void __fastcall InitialiseActionPause(CCreatureActionBase* action) {
    CCreatureActionBaseVirtuals* virtuals =
        reinterpret_cast<CCreatureActionBaseVirtuals*>(action);
    int actionId = virtuals->GetActionId();
    if (actionId > 0) {
        ActionRegistrar* registrar = GetActionRegistrar();
        registrar->RegisterAction(actionId, (void*)0x13bac90);
    }
}
