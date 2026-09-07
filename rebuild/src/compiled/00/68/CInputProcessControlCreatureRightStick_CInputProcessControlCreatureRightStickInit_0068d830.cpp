
#include "engine/CInputProcessControlCreatureRightStick.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall CInputProcessControlCreatureRightStick_Init(CInputProcessControlCreatureRightStick* self) {
    self->PushingCameraModeUp = 0;
    self->PushingCameraModeDown = 0;
    self->LookingAround = 0;
}