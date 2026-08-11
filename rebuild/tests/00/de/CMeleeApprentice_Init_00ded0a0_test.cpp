#include <stdio.h>
#include <string.h>

struct CMeleeApprentice {
    char pad[0x48];
    unsigned char f48;
    unsigned char f49;
    unsigned char f4a;
    unsigned char f4b;
    unsigned char f4c;
    unsigned char f4d;
    unsigned char f4e;
    unsigned char f4f;
};

void __fastcall CMeleeApprentice_Init(CMeleeApprentice *self);

int main()
{
    CMeleeApprentice o;
    memset(&o, 0xAA, sizeof(o));
    CMeleeApprentice_Init(&o);
    if (o.f48==0 && o.f49==0 && o.f4a==0 && o.f4b==0 &&
        o.f4c==0 && o.f4d==0 && o.f4f==0 && o.f4e==0xAA)
    {
        printf("00ded0a0_TEST PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    return 0;
}