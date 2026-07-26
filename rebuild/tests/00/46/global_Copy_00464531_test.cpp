#include "rebuild_abi.h"
#include <cstdio>

/* Self-contained parity test for the 0x00464531 jmp thunk.
 * We can't exercise the masked callee, so we validate that the thunk
 * forwards to a stub with the same signature (tail-call semantics). */

struct CThingPhysicalSwitchDef { int tag; };

static int g_called = 0;
static CThingPhysicalSwitchDef* g_this = 0;
static void* g_src = 0;

extern void __fastcall CThingPhysicalSwitchDef_Copy_impl(
    CThingPhysicalSwitchDef* thisptr, void* src)
{
    g_called = 1;
    g_this = thisptr;
    g_src = src;
}

void __fastcall CThingPhysicalSwitchDef_Copy(
    CThingPhysicalSwitchDef* thisptr, void* src);

int main(void)
{
    CThingPhysicalSwitchDef obj;
    obj.tag = 0x77;
    int srcbuf = 42;

    CThingPhysicalSwitchDef_Copy(&obj, &srcbuf);

    if (g_called == 1 && g_this == &obj && g_src == &srcbuf) {
        printf("OK_464531\n");
    } else {
        printf("FAIL_464531\n");
    }
    return 0;
}