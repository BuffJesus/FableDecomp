#include <cstdio>

class CTCPhysicsStandard
{
public:
    unsigned char pad0[0x1c8];
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;

    bool __fastcall CanCollideWithLandscape(void* /*edx*/);
};

bool __fastcall CTCPhysicsStandard::CanCollideWithLandscape(void* /*edx*/)
{
    return bit4;
}

int main()
{
    CTCPhysicsStandard obj;
    unsigned char* p = (unsigned char*)&obj;
    for (int i = 0; i < (int)sizeof(obj); i++) p[i] = 0;

    p[0x1c8] = 0x00;
    bool r0 = obj.CanCollideWithLandscape(0);

    p[0x1c8] = 0x10;
    bool r1 = obj.CanCollideWithLandscape(0);

    p[0x1c8] = 0xEF;
    bool r2 = obj.CanCollideWithLandscape(0);

    p[0x1c8] = 0xFF;
    bool r3 = obj.CanCollideWithLandscape(0);

    if (r0 == false && r1 == true && r2 == false && r3 == true)
    {
        printf("CANCOLLIDE_OK\n");
        return 0;
    }

    printf("CANCOLLIDE_FAIL %d %d %d %d\n", (int)r0, (int)r1, (int)r2, (int)r3);
    return 1;
}