#include "engine/CCreatureAction_CastStrengthMultiStrike.h"

struct CCreatureAction_CastStrengthMultiStrike_Methods : CCreatureAction_CastStrengthMultiStrike {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void DoStrike();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual bool ShouldStrike();
    virtual void FrameUpdate();
};
void __fastcall CCreatureAction_Base_FrameUpdate(CCreatureAction_CastStrengthMultiStrike* action);

void CCreatureAction_CastStrengthMultiStrike_Methods::FrameUpdate() {
    CCreatureAction_Base_FrameUpdate(
        reinterpret_cast<CCreatureAction_CastStrengthMultiStrike*>(this));
    if (ShouldStrike())
        DoStrike();
}
