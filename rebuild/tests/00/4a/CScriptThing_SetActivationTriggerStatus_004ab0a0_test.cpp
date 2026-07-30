#include <stdio.h>

// ---- function under test (naked forwarder) ----
__declspec(naked) void __fastcall SetActivationTriggerStatus(void* ecx, void* edx, int status)
{
    (void)ecx; (void)edx; (void)status;
    __asm {
        mov  ecx, dword ptr [ecx+4]
        test ecx, ecx
        je   short l_done
        mov  eax, dword ptr [ecx]
        jmp  dword ptr [eax+0x114]
    l_done:
        ret  4
    }
}

// ---- test harness ----
static int g_called = 0;
static int g_arg = -999;

// virtual method target sitting at vtable slot 0x114 (=offset 0x114 bytes)
static void __fastcall Target(void* self, void* edx, int status)
{
    (void)self; (void)edx;
    g_called = 1;
    g_arg = status;
}

// vtable: an array of function pointers; slot at byte offset 0x114 must be Target.
static void* g_vtbl[0x114/4 + 1];

struct Member { void** vptr; };

// outer object layout: +0 unused, +4 = pointer to member (or null)
struct Outer { int pad0; Member* member; };

int main()
{
    // build vtable
    for (int i = 0; i < (int)(sizeof(g_vtbl)/sizeof(g_vtbl[0])); ++i) g_vtbl[i] = 0;
    g_vtbl[0x114/4] = (void*)&Target;

    Member m;
    m.vptr = g_vtbl;

    Outer obj;
    obj.pad0 = 0;
    obj.member = &m;

    // --- case 1: member non-null -> should forward to Target with status ---
    void* pthis = &obj;
    int status = 7;
    __asm {
        mov ecx, pthis
        push status
        call SetActivationTriggerStatus
    }
    if (!g_called || g_arg != 7) { printf("FAIL forward: called=%d arg=%d\n", g_called, g_arg); return 1; }

    // --- case 2: member null -> should just return, not call ---
    g_called = 0; g_arg = -999;
    Outer obj2;
    obj2.pad0 = 0;
    obj2.member = 0;
    void* pthis2 = &obj2;
    int status2 = 3;
    __asm {
        mov ecx, pthis2
        push status2
        call SetActivationTriggerStatus
    }
    if (g_called != 0) { printf("FAIL null-path called Target\n"); return 1; }

    printf("OK_0x004ab0a0 forward arg=7, null-safe\n");
    return 0;
}