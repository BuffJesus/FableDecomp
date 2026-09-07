#include "engine/CJoystickDX.h"
#include <stdio.h>

struct C2DVector { float x; float y; };


void __fastcall CJoystickDX_SetAxes(CJoystickDX* self, int /*edx*/, const C2DVector& v)
{
    self->f = v.x;
    self->f_2 = v.y;
}

int main()
{
    CJoystickDX obj;
    obj.f = -1.0f;
    obj.f_2 = -1.0f;
    C2DVector v;
    v.x = 3.5f;
    v.y = 7.25f;
    CJoystickDX_SetAxes(&obj, 0, v);
    if (obj.f == 3.5f && obj.f_2 == 7.25f) {
        printf("00ab78e0_TEST PASS\n");
    } else {
        printf("FAIL %f %f\n", obj.f, obj.f_2);
    }
    return 0;
}