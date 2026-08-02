#include <stdio.h>

struct CTCPhysicsStandard
{
    char pad[0x1c8];
    unsigned char flags1c8;
};

bool __fastcall CTCPhysicsStandard_CanCollideWithLandscape(CTCPhysicsStandard* self, void* /*edx*/)
{
    unsigned char v = self->flags1c8;
    v = v >> 3;
    v = v & 1;
    return v;
}

int main(void)
{
    CTCPhysicsStandard obj;
    for (int i = 0; i < (int)sizeof(obj); i++)
        ((char*)&obj)[i] = 0;

    obj.flags1c8 = 0x00;
    bool r0 = CTCPhysicsStandard_CanCollideWithLandscape(&obj, 0);

    obj.flags1c8 = 0x08;
    bool r1 = CTCPhysicsStandard_CanCollideWithLandscape(&obj, 0);

    obj.flags1c8 = 0xF7;
    bool r2 = CTCPhysicsStandard_CanCollideWithLandscape(&obj, 0);

    obj.flags1c8 = 0xFF;
    bool r3 = CTCPhysicsStandard_CanCollideWithLandscape(&obj, 0);

    if (r0 == false && r1 == true && r2 == false && r3 == true)
    {
        printf("CANCOLLIDE_OK\n");
        return 0;
    }

    printf("CANCOLLIDE_FAIL %d %d %d %d\n", (int)r0, (int)r1, (int)r2, (int)r3);
    return 1;
}