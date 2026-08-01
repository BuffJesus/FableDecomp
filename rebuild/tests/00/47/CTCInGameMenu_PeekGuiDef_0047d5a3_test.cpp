#include <cstdio>
#include "rebuild_abi.h"

// Standalone harness for CTCInGameMenu::PeekGuiDef @ 0x0047d5a3.
// The real EH prolog/epilog and engine callees are relocation-masked in parity;
// here we provide stack-balanced benign stubs so the naked thunk executes to
// completion deterministically, and observe the side effects through sentinel
// globals. The produced object is a fake CountedPointer whose refcount is >1 so
// the teardown decrements (to a nonzero value) and skips the virtual-dtor slot.

extern "C" char g_funcinfo_0047d5a3[8];
char g_funcinfo_0047d5a3[8] = {0};

static int g_prolog_hit = 0;
static int g_producer_hit = 0;
static int g_method_hit = 0;
static int g_epilog_hit = 0;

// Fake CountedPointer target: [+0] vtable ptr, [+4] refcount.
static int g_fake_vtable[4] = {0, 0, 0, 0};
static void* g_fake_object[2];

// EH_prolog: consume the two pushed args (-1, funcinfo) and return net -8 so
// the ebp frame the naked body itself manages (push ebp / leave) rebalances.
extern "C" __declspec(naked) void FABLE_CDECL EH_prolog_0047d5a3(void)
{
    __asm
    {
        pop  eax            ; eax = return addr
        add  esp, 8         ; discard the two pushed args (-1, funcinfo)
        push eax            ; restore return addr
        inc  dword ptr [g_prolog_hit]
        ret
    }
}

// G_producer(&result, 1, &temp) -> object ptr in eax; callee-cleans 3 stack
// args (0xC bytes), matching retail (no add esp after the call site). Retail G
// stores the acquired CountedPointer into *(&result); mirror that so the
// teardown path (esi = result holder) is exercised.
// Stack at entry: [esp+0]=retaddr, [esp+4]=&temp, [esp+8]=1, [esp+0Ch]=&result.
extern "C" __declspec(naked) void FABLE_CDECL G_producer_0047d5a3(void)
{
    __asm
    {
        inc  dword ptr [g_producer_hit]
        mov  eax, dword ptr [esp+0Ch]   ; &result
        lea  ecx, [g_fake_object]
        mov  dword ptr [eax], ecx       ; *result = &fake_object
        mov  eax, ecx                   ; return object ptr in eax
        ret  0Ch
    }
}

// Method(ecx=this): __fastcall, no stack args.
extern "C" __declspec(naked) void FABLE_CDECL Method_0047d5a3(void)
{
    __asm
    {
        inc  dword ptr [g_method_hit]
        ret
    }
}

// EH_epilog: net-zero on esp (SEH record teardown only).
extern "C" __declspec(naked) void FABLE_CDECL EH_epilog_0047d5a3(void)
{
    __asm
    {
        inc  dword ptr [g_epilog_hit]
        ret
    }
}

extern "C" void* FABLE_FASTCALL PeekGuiDef_0047d5a3(void* self);

int main(void)
{
    // Wire the fake object: vtable at [+0], refcount 2 at [+4].
    g_fake_object[0] = (void*)g_fake_vtable;
    g_fake_object[1] = (void*)2;

    void* self = (void*)0;
    void* ret = PeekGuiDef_0047d5a3(self);

    int refcount_after = (int)(size_t)g_fake_object[1];

    if (g_prolog_hit == 1 && g_producer_hit == 1 &&
        g_method_hit == 1 && g_epilog_hit == 1 &&
        ret == (void*)g_fake_object && refcount_after == 1)
    {
        printf("PEEKGUIDEF_0047D5A3_TEST PASS\n");
    }
    else
    {
        printf("FAIL_0047d5a3 p=%d g=%d m=%d e=%d ret=%p rc=%d\n",
               g_prolog_hit, g_producer_hit, g_method_hit, g_epilog_hit,
               ret, refcount_after);
    }
    return 0;
}