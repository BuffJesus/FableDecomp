#include "engine/CTCBoastingPosition.h"

char __fastcall IsPositionActivated(CTCBoastingPosition* self, int /*edx*/, long* out)
{
    if (self->PositionActivated) {
        *out = self->BoastIndex;
    }
    return self->PositionActivated;
}
