#include <stdio.h>

struct CTCInventoryBaseVtbl {
    void* slot[47];   // slot[46] = +0xb8 = the virtual method
};

struct CTCInventoryBase {
    CTCInventoryBaseVtbl* vtbl; // +0x00
    char pad[0x1c - 0x04];
    int  state;                 // +0x1c
};

// function under test (naked, this=ecx, no args)
__declspec(naked) void ProcessButtonAReleased()
{
    __asm {
        mov  eax, dword ptr [ecx+0x1c]
        dec  eax
        jne  end
        mov  eax, dword ptr [ecx]
        push 0
        call dword ptr [eax+0xb8]
    end:
        ret
    }
}

// ---- test harness ----
static int   g_calls = 0;
static int   g_lastArg = -999;
static void* g_lastSelf = 0;

// Real ABI of the vtable target: this in ecx, one arg on the stack, callee
// cleans that arg (ret 4). Written naked so no compiler ABI rewrite distorts it.
__declspec(naked) void FakeDoActivate()
{
    __asm {
        mov  eax, ecx                 ; this
        mov  g_lastSelf, eax
        mov  eax, dword ptr [esp+4]   ; stack arg
        mov  g_lastArg, eax
        inc  g_calls
        ret  4                        ; clean 1 stack arg
    }
}

// invoke naked fn with this in ecx
static void Call(CTCInventoryBase* self)
{
    void* p = self;
    void (*fn)() = ProcessButtonAReleased;
    __asm {
        mov ecx, p
        call fn
    }
}

int main()
{
    CTCInventoryBaseVtbl vt;
    for (int i = 0; i < 47; ++i) vt.slot[i] = 0;
    vt.slot[46] = (void*)&FakeDoActivate;

    CTCInventoryBase obj;
    obj.vtbl = &vt;

    // state != 1: no call
    obj.state = 0;
    Call(&obj);
    if (g_calls != 0) { printf("FAIL: called when state!=1\n"); return 1; }

    // state == 1: exactly one call, arg 0, correct self
    obj.state = 1;
    Call(&obj);
    if (g_calls != 1)             { printf("FAIL: call count %d\n", g_calls); return 1; }
    if (g_lastArg != 0)           { printf("FAIL: arg %d\n", g_lastArg); return 1; }
    if (g_lastSelf != (void*)&obj){ printf("FAIL: self mismatch\n"); return 1; }

    // state == 2: no additional call
    obj.state = 2;
    Call(&obj);
    if (g_calls != 1) { printf("FAIL: extra call at state 2\n"); return 1; }

    printf("OK_0x005bc638\n");
    return 0;
}