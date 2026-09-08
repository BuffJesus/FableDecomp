#include "engine/CCreatureAction_UnsheatheWeapons.h"

struct CCreatureAction_UnsheatheWeapons_Methods : CCreatureAction_UnsheatheWeapons {
    virtual void v0(); virtual void v1(); virtual void v2();
    virtual void OnFinishHook();
    virtual void FinishAction();
};
bool __fastcall UnsheatheWeapons_CheckPredicate(CCreatureAction_UnsheatheWeapons_Methods* action);
void __fastcall UnsheatheWeapons_BaseFinishAction(CCreatureAction_UnsheatheWeapons_Methods* action);

void CCreatureAction_UnsheatheWeapons_Methods::FinishAction() {
    if (!UnsheatheWeapons_CheckPredicate(this))
        OnFinishHook();
    UnsheatheWeapons_BaseFinishAction(this);
}
