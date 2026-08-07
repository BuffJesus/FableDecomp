#include "rebuild_abi.h"
#include <cstdio>
static int g_help = 0;
struct FableCarry_006a2520
{
    char pad[0x1c];
    unsigned char field_1c;
    char pad2[3];
    unsigned long field_20;
    void helper();
    void SetAll(unsigned long arg);
};
void FableCarry_006a2520::helper() { g_help++; }
int main()
{
    static FableCarry_006a2520 c;
    c.field_1c = 0; c.field_20 = 0; g_help = 0;
    c.SetAll(0x99);
    if (c.field_1c != 1) { std::printf("BAD_FLAG\n"); return 1; }
    if (c.field_20 != 0x99) { std::printf("BAD_VAL\n"); return 1; }
    if (g_help != 1) { std::printf("BAD_HELP\n"); return 1; }
    std::printf("SETCARRY_006a2520_OK\n");
    return 0;
}