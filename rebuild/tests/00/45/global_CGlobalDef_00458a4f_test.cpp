#include "rebuild_abi.h"
#include <cstdio>

struct CGlobalDef;

static int g_calls = 0;
static unsigned long g_last_shared = 0;

extern "C" void __fastcall CGlobalDef_dtor_10c(void* p){ (void)p; g_calls++; }
extern "C" void __fastcall CGlobalDef_dtor_100(void* p){ (void)p; g_calls++; }
extern "C" void __fastcall CGlobalDef_dtor_0f4(void* p){ (void)p; g_calls++; }
extern "C" void __fastcall CGlobalDef_dtor_shared(void* p){ g_calls++; g_last_shared = (unsigned long)p; }
extern "C" void __fastcall CGlobalDef_base_dtor(void* p){ (void)p; g_calls++; }

extern "C" void __fastcall CGlobalDef_dtor(CGlobalDef* thisptr);

int main()
{
    static unsigned char blob[0x200];
    CGlobalDef_dtor(reinterpret_cast<CGlobalDef*>(blob));
    // 3 unique + 4 shared + 1 base = 8 calls; last shared arg = blob+0x90
    if (g_calls == 8 && g_last_shared == (unsigned long)(blob + 0x90))
        printf("OK_00458a4f\n");
    else
        printf("FAIL %d\n", g_calls);
    return 0;
}