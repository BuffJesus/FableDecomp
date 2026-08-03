#include <stdio.h>

// ---- function under test ----
__declspec(naked) int IsPosChangeable(int pos)
{
    (void)pos;
    __asm {
        mov     eax, dword ptr [esp+4]
        push    esi
        push    edi
        mov     esi, ecx
        mov     edi, dword ptr [esi]
        push    eax
        call    dword ptr [edi+0x44]
        push    eax
        mov     ecx, esi
        call    dword ptr [edi+0x60]
        pop     edi
        pop     esi
        ret     4
    }
}

// ---- fake object with a vtable big enough for slot 0x60 (24) ----
static int g_lastVf44Arg = -1;
static int g_lastVf60Arg = -1;

// vf44: __fastcall(this=ecx, arg). return arg*2+1
__declspec(naked) static int myVf44()
{
    __asm {
        mov     eax, dword ptr [esp+4]   ; arg
        mov     g_lastVf44Arg, eax
        add     eax, eax
        inc     eax
        ret     4
    }
}
// vf60: __fastcall(this=ecx, arg). return arg+100 (proves it received vf44 result)
__declspec(naked) static int myVf60()
{
    __asm {
        mov     eax, dword ptr [esp+4]
        mov     g_lastVf60Arg, eax
        add     eax, 100
        ret     4
    }
}

struct Obj { void* vtbl; };

int main()
{
    // vtable: need indices 0x44/4=17 and 0x60/4=24
    static void* vtbl[40];
    for (int i=0;i<40;i++) vtbl[i]=0;
    vtbl[0x44/4] = (void*)&myVf44;
    vtbl[0x60/4] = (void*)&myVf60;

    Obj o; o.vtbl = vtbl;

    int pos = 7;
    int result;
    Obj* pobj = &o;
    __asm {
        mov     ecx, pobj
        push    pos
        call    IsPosChangeable
        mov     result, eax
    }

    // vf44 got pos=7 -> 15 ; vf60 got 15 -> 115
    if (g_lastVf44Arg == 7 && g_lastVf60Arg == 15 && result == 115) {
        printf("OK_0x004fb2f0 result=%d vf44arg=%d vf60arg=%d\n", result, g_lastVf44Arg, g_lastVf60Arg);
        return 0;
    }
    printf("FAIL result=%d vf44arg=%d vf60arg=%d\n", result, g_lastVf44Arg, g_lastVf60Arg);
    return 1;
}