#include <stdio.h>

struct CGenericVertex_LandscapeBg
{
    unsigned char pad[0x44];
    unsigned char flags;

    unsigned int GetU1();
};

unsigned int CGenericVertex_LandscapeBg::GetU1()
{
    unsigned int v = flags;
    v = v >> 2;
    v = v & 1;
    return v;
}

int main()
{
    CGenericVertex_LandscapeBg obj;
    unsigned char *rawByte = (unsigned char *)&obj + 0x44;
    for (unsigned int v = 0; v < 256; v++)
    {
        *rawByte = (unsigned char)v;
        unsigned int expected = (v >> 2) & 1;
        unsigned int actual = obj.GetU1();
        if (actual != expected)
        {
            printf("MISMATCH at v=%u expected=%u actual=%u\n", v, expected, actual);
            return 1;
        }
    }
    printf("GENVTX_U1_OK\n");
    return 0;
}