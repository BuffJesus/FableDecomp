#include <cstdio>

struct CTCPhysicsStandard { char pad[0x15d]; unsigned char flags; };

unsigned char __fastcall CTCPhysicsStandard_CanCollideWithLandscape(const CTCPhysicsStandard* self)
{
    unsigned char v = self->flags;
    v = (unsigned char)(v >> 2);
    v = (unsigned char)(v & 1);
    return v;
}

int main()
{
    CTCPhysicsStandard o;
    o.flags = 0x04;
    if (CTCPhysicsStandard_CanCollideWithLandscape(&o) != 1) { std::printf("FAIL bit set\n"); return 1; }
    o.flags = 0x00;
    if (CTCPhysicsStandard_CanCollideWithLandscape(&o) != 0) { std::printf("FAIL bit clear\n"); return 1; }
    o.flags = 0xFB;
    if (CTCPhysicsStandard_CanCollideWithLandscape(&o) != 0) { std::printf("FAIL masked other bits\n"); return 1; }
    o.flags = 0xFF;
    if (CTCPhysicsStandard_CanCollideWithLandscape(&o) != 1) { std::printf("FAIL all bits\n"); return 1; }
    std::printf("CTCPhysicsStandard_00661e70_TEST PASS\n");
    return 0;
}