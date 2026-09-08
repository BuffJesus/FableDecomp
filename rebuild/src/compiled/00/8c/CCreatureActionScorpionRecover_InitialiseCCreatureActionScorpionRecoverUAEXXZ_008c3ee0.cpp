#include "engine/CCreatureAction_ScorpionRecover.h"

struct ScorpionRecoverTarget {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void SetRecovering(int enabled);
};
struct CreaturePointerStorage { ScorpionRecoverTarget* GetTarget(); };
struct CCreatureAction_ScorpionRecover_Methods : CCreatureAction_ScorpionRecover {
    void BaseInit();
    virtual void Initialise();
};

void CCreatureAction_ScorpionRecover_Methods::Initialise() {
    BaseInit();
    CCreatureAction_ScorpionRecover* action =
        reinterpret_cast<CCreatureAction_ScorpionRecover*>(this);
    CreaturePointerStorage* creature =
        reinterpret_cast<CreaturePointerStorage*>(&action->m8);
    ScorpionRecoverTarget* target = creature->GetTarget();
    target->SetRecovering(1);
}
