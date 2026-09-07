#include "engine/CCreatureAction_GenericAimProjectileWeapon.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_GenericAimProjectileWeapon; void __fastcall oal_sub(CCreatureAction_GenericAimProjectileWeapon* self);
struct CCreatureAction_GenericAimProjectileWeapon_Methods : CCreatureAction_GenericAimProjectileWeapon {
    void M();
};
void CCreatureAction_GenericAimProjectileWeapon_Methods::M(){ oal_sub(this); this->f10 = 7; }