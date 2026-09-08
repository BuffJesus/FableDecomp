#include "engine/CCreatureAction_PickCrop.h"

struct CPickCropActionState {
    int PerformStep();
};

struct CCreatureAction_PickCrop_Methods : CCreatureAction_PickCrop {
    void PerformAction();
};

void CCreatureAction_PickCrop_Methods::PerformAction()
{
    CPickCropActionState* state = (CPickCropActionState*)&this->sub;
    if (state->PerformStep()) {
        state->PerformStep();
    }
}
