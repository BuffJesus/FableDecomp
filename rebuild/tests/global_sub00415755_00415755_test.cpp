#include <cstdio>
#include "rebuild_abi.h"

// Standalone harness for sub_00415755. The real EH prolog/epilog and engine
// callees are relocation-masked in parity; here we provide stack-balanced
// benign stubs so the naked thunk executes to completion deterministically,
// and observe the side effects through sentinel globals.
//
// Verified: compiled + linked (VS x86 cl) and run -> prints PASS_00415755_OK,
// confirming balanced stack and all four call sites hit exactly once.

extern "C" char g_scopetable_00415755[8];
char g_scopetable_00415755[8] = {0};

static int g_prolog_hit = 0;
static int g_producer_hit = 0;
static int g_method_hit = 0;
static int g_epilog_hit = 0;
static int g_fake_object = 0;

// EH_prolog3: consume the two pushed args and carve a frame sized so the
// caller's trailing `pop esi; add esp,0x20; ret` rebalances exactly.
extern "C" __declspec(naked) void FABLE_CDECL EH_prolog3_00415755(void)
{
    __asm
    {
        pop  eax            ; eax = return addr
        add  esp, 8         ; discard the two pushed args (-1, scope)
        sub  esp, 0x1c      ; carve frame so tail pop esi/add esp,0x20/ret balances
        push eax            ; restore return addr
        inc  dword ptr [g_prolog_hit]
        ret
    }
}

// F_producer(esi, &out) -> object ptr in eax; leaves its stack args for the
// frame teardown to sweep (plain ret).
extern "C" __declspec(naked) void FABLE_CDECL F_producer_00415755(void)
{
    __asm
    {
        inc  dword ptr [g_producer_hit]
        lea  eax, [g_fake_object]
        ret
    }
}

// Method(ecx=this): __fastcall, no stack args.
extern "C" __declspec(naked) void FABLE_CDECL Method_00415755(void)
{
    __asm
    {
        inc  dword ptr [g_method_hit]
        ret
    }
}

// EH_epilog3: tear down the SEH record only (net-zero on esp); the caller's
// trailing `pop esi; add esp,0x20` releases the frame carved by prolog3.
extern "C" __declspec(naked) void FABLE_CDECL EH_epilog3_00415755(void)
{
    __asm
    {
        inc  dword ptr [g_epilog_hit]
        ret
    }
}

extern "C" void FABLE_FASTCALL sub_00415755(void);

int main(void)
{
    __asm { mov esi, 0 }   // benign producer context
    sub_00415755();

    if (g_prolog_hit == 1 && g_producer_hit == 1 &&
        g_method_hit == 1 && g_epilog_hit == 1)
    {
        printf("PASS_00415755_OK\n");
    }
    else
    {
        printf("FAIL_00415755 p=%d f=%d m=%d e=%d\n",
               g_prolog_hit, g_producer_hit, g_method_hit, g_epilog_hit);
    }
    return 0;
}