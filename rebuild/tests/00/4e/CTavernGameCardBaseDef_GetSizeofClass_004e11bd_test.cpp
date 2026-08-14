#include <cstdio>

struct CTavernGameCardBaseDef;

unsigned long __fastcall GetSizeofClass_model(CTavernGameCardBaseDef *self)
{
    (void)self;
    return 0x84u;
}

int main()
{
    unsigned long v = GetSizeofClass_model((CTavernGameCardBaseDef*)0);
    if (v == 0x84u)
        printf("PARITY_OK\n");
    else
        printf("PARITY_BAD %lu\n", v);
    return 0;
}