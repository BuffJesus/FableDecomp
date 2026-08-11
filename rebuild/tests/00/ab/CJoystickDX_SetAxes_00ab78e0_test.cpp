#include <stdio.h>

struct C2DVector { float x; float y; };

struct CJoystickDX {
    char pad[0xd18];
    float fd18;
    float fd1c;
};

void __fastcall CJoystickDX_SetAxes(CJoystickDX* self, int /*edx*/, const C2DVector& v)
{
    self->fd18 = v.x;
    self->fd1c = v.y;
}

int main()
{
    CJoystickDX obj;
    obj.fd18 = -1.0f;
    obj.fd1c = -1.0f;
    C2DVector v;
    v.x = 3.5f;
    v.y = 7.25f;
    CJoystickDX_SetAxes(&obj, 0, v);
    if (obj.fd18 == 3.5f && obj.fd1c == 7.25f) {
        printf("00ab78e0_TEST PASS\n");
    } else {
        printf("FAIL %f %f\n", obj.fd18, obj.fd1c);
    }
    return 0;
}