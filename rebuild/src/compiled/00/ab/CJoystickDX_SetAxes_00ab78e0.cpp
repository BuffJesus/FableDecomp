#include "engine/CJoystickDX.h"  // retyped onto the PDB layout; byte parity re-verified
struct C2DVector { float x; float y; };


void __fastcall CJoystickDX_SetAxes(CJoystickDX* self, int /*edx*/, const C2DVector& v)
{
    self->f = v.x;
    self->f_2 = v.y;
}