#include "rebuild_abi.h"
#include <cstdio>
struct FableClickable_0055ac90
{
    char pad30[0x30];
    unsigned long field_30;
    char pad[0x190 - 0x34];
    unsigned long field_190;
    void FABLE_FASTCALL base_init();
};
static int g_base = 0;
void FABLE_FASTCALL FableClickable_0055ac90::base_init() { g_base++; }
extern "C" void FABLE_FASTCALL FableClickableInit_0055ac90(FableClickable_0055ac90* self);
int main()
{
    FableClickable_0055ac90 c;
    c.field_30 = 0xdeadbeef; c.field_190 = 0;
    FableClickableInit_0055ac90(&c);
    if (g_base != 1) { std::printf("BAD_BASE\n"); return 1; }
    if (c.field_190 != 0xdeadbeef) { std::printf("BAD_COPY\n"); return 1; }
    std::printf("CLICKINIT_0055ac90_OK\n");
    return 0;
}