struct C3DVector_0066fae0
{
    unsigned long x;
    unsigned long y;
    unsigned long z;
};

struct CTCCoopSpirit_0066fae0
{
    unsigned char prefix[0x18];
    C3DVector_0066fae0 movement;
};

extern "C" void __fastcall
CTCCoopSpirit_ApplyMovementVector_0066fae0(
    CTCCoopSpirit_0066fae0* self,
    void*,
    const C3DVector_0066fae0* movement)
{
    self->movement = *movement;
}
