#include <cstdio>

// ---- function under test (verbatim source) ----
extern "C" void InternalChanged_tail(void);

__declspec(naked) void InternalChanged()
{
    __asm {
        push esi
        mov  esi, ecx
        mov  ecx, dword ptr [esi+0x148]
        mov  eax, dword ptr [esi]
        push ecx
        mov  ecx, esi
        call dword ptr [eax+0x21c]
        mov  ecx, esi
        pop  esi
        jmp  InternalChanged_tail
    }
}

// ---- observation state ----
static void*        g_vcall_this   = 0;
static unsigned int g_vcall_arg    = 0;
static int          g_vcall_hits   = 0;
static void*        g_tail_this    = 0;
static int          g_tail_hits    = 0;

// virtual method installed at vtable slot 0x21c: __fastcall(this=ecx, arg on stack)
// captures this and the pushed argument, then returns (ret 4 to clean the one stack arg)
static __declspec(naked) void vmeth_21c()
{
    __asm {
        mov  eax, ecx                 ; this
        mov  g_vcall_this, eax
        mov  eax, dword ptr [esp+4]   ; the pushed arg (= field_148)
        mov  g_vcall_arg, eax
        inc  dword ptr g_vcall_hits
        ret  4
    }
}

// tail target 0x52c780: __fastcall(this=ecx). Captures this. Real linkage name InternalChanged_tail.
extern "C" __declspec(naked) void InternalChanged_tail()
{
    __asm {
        mov  eax, ecx
        mov  g_tail_this, eax
        inc  dword ptr g_tail_hits
        ret
    }
}

// object layout: [0]=vtable ptr, ... , [0x148]=field_148
struct Obj {
    void** vtbl;
    char   pad[0x148 - sizeof(void*)];
    unsigned int field_148;
};

int main()
{
    // vtable big enough to hold slot at byte offset 0x21c (index 135)
    static void* vt[200];
    for (int i = 0; i < 200; ++i) vt[i] = 0;
    vt[0x21c / 4] = (void*)&vmeth_21c;

    Obj obj;
    obj.vtbl = vt;
    obj.field_148 = 0xDEADBEEF;

    g_vcall_this = 0; g_vcall_arg = 0; g_vcall_hits = 0;
    g_tail_this = 0; g_tail_hits = 0;

    void* pthis = &obj;
    __asm {
        mov ecx, pthis
        call InternalChanged
    }

    if (g_vcall_hits != 1)                       { printf("FAIL vcall_hits=%d\n", g_vcall_hits); return 1; }
    if (g_vcall_this != (void*)&obj)             { printf("FAIL vcall_this\n"); return 1; }
    if (g_vcall_arg != 0xDEADBEEF)               { printf("FAIL vcall_arg=%08x\n", g_vcall_arg); return 1; }
    if (g_tail_hits != 1)                        { printf("FAIL tail_hits=%d\n", g_tail_hits); return 1; }
    if (g_tail_this != (void*)&obj)              { printf("FAIL tail_this\n"); return 1; }

    printf("OK_0x00554110\n");
    return 0;
}