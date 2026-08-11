struct C2DVector { float x; float y; };

struct CJoystickDX {
    char pad[0xd18];
    float fd18; // offset 0xd18
    float fd1c; // offset 0xd1c
};

void __fastcall CJoystickDX_SetAxes(CJoystickDX* self, int /*edx*/, const C2DVector& v)
{
    self->fd18 = v.x;
    self->fd1c = v.y;
}