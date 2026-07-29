#include <stdio.h>
#include <string.h>

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
    unsigned long suffix;
};

extern "C" void __fastcall
CTCCoopSpirit_ApplyMovementVector_0066fae0(
    CTCCoopSpirit_0066fae0* self,
    void*,
    const C3DVector_0066fae0* movement);

static int Check(int condition, const char* message)
{
    if (!condition)
    {
        printf("FAIL: %s\n", message);
        return 0;
    }
    return 1;
}

int main()
{
    int ok = 1;
    CTCCoopSpirit_0066fae0 spirit;
    C3DVector_0066fae0 movement;

    memset(&spirit, 0x5a, sizeof(spirit));
    movement.x = 0x3f800000;
    movement.y = 0xc0200000;
    movement.z = 0x7fc12345;

    CTCCoopSpirit_ApplyMovementVector_0066fae0(
        &spirit, 0, &movement);

    ok &= Check(spirit.movement.x == movement.x &&
                spirit.movement.y == movement.y &&
                spirit.movement.z == movement.z,
                "all three vector words must be copied exactly");
    ok &= Check(spirit.prefix[0] == 0x5a &&
                spirit.prefix[0x17] == 0x5a &&
                spirit.suffix == 0x5a5a5a5a,
                "copy must preserve adjacent object state");
    ok &= Check(movement.x == 0x3f800000 &&
                movement.y == 0xc0200000 &&
                movement.z == 0x7fc12345,
                "copy must not mutate the source vector");

    if (!ok)
        return 1;

    puts("PASS CTCCoopSpirit::ApplyMovementVector 0x0066FAE0");
    return 0;
}
