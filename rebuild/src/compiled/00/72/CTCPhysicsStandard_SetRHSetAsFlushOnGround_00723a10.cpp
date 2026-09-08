#include "engine/CTCPhysicsStandard.h"  // retyped onto the PDB layout; byte parity re-verified

struct CPhysicsVector { int X, Y, Z; };
struct CFlushGroundHelper;

struct CTCPhysicsStandard_Methods : CTCPhysicsStandard {
    CFlushGroundHelper* ComputeFlushGround(
        CPhysicsVector* out, CPhysicsVector* groundVector);
};

extern void __fastcall Helper_use(CFlushGroundHelper* helper);

void __fastcall CTCPhysicsStandard_SetRHSetAsFlushOnGround(
    CTCPhysicsStandard_Methods* self)
{
    CPhysicsVector flushVector;
    CFlushGroundHelper* helper = self->ComputeFlushGround(
        &flushVector, (CPhysicsVector*)&self->ground);
    Helper_use(helper);
    typedef void (CTCPhysicsStandard_Methods::*SetFlushGroundFn)(CPhysicsVector*);
    SetFlushGroundFn method =
        *(SetFlushGroundFn*)((char*)self->__vftable + 0x134);
    (self->*method)(&flushVector);
}
