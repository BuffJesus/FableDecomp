#include <stdio.h>

struct CTCPhysicsStandard
{
    char pad_0000[0x1c8];
    unsigned char flags1c8;

    unsigned char CanCollideWithLandscape_a();
};

unsigned char CTCPhysicsStandard::CanCollideWithLandscape_a()
{
    unsigned char v = flags1c8;
    v = v >> 2;
    v = v & 1;
    return v;
}

int main()
{
    CTCPhysicsStandard obj;
    unsigned char *rawByte = (unsigned char *)&obj + 0x1c8;
    for (unsigned int v = 0; v < 256; v++)
    {
        *rawByte = (unsigned char)v;
        bool expected = (v >> 2) & 1;
        bool actual = obj.CanCollideWithLandscape_a();
        if (actual != expected)
        {
            printf("MISMATCH at v=%u expected=%d actual=%d\n", v, (int)expected, (int)actual);
            return 1;
        }
    }
    printf("CANCOLLIDE_TEST_OK\n");
    return 0;
}