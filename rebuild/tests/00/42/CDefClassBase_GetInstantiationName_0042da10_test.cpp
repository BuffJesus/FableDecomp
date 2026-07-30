#include <stdio.h>

int * __stdcall GetInstantiationName(int *out)
{
    *out = -1;
    return out;
}

int main(void)
{
    int slot = 0x12345678;
    int *r = GetInstantiationName(&slot);
    if (slot != -1) { printf("FAIL slot=%d\n", slot); return 1; }
    if (r != &slot) { printf("FAIL ret\n"); return 1; }
    printf("OK_0x0042da10\n");
    return 0;
}