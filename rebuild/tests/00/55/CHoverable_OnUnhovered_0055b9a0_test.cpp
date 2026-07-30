#include <cstdio>
#include <cstring>

// ---- function under test (same source as candidate) ----
__declspec(naked) void OnUnhovered()
{
    __asm {
        push esi
        mov  esi, ecx
        mov  ecx, dword ptr [esi+0x15c]
        mov  eax, dword ptr [esi]
        push ecx
        mov  ecx, esi
        call dword ptr [eax+0xc0]
        mov  eax, dword ptr [esi+0x168]
        mov  edx, dword ptr [esi]
        push eax
        mov  ecx, esi
        call dword ptr [edx+0x20c]
        pop  esi
        ret
    }
}

// ---- observation of the two virtual calls ----
static int g_call0_arg = 0;   // arg passed to vf @ +0xc0
static int g_call0_this = 0;
static int g_call1_arg = 0;   // arg passed to vf @ +0x20c
static int g_call1_this = 0;

// __fastcall stubs: this in ecx, one stack arg. ret 4.
__declspec(naked) static void vf_at_c0()
{
    __asm {
        mov g_call0_this, ecx
        mov eax, dword ptr [esp+4]
        mov g_call0_arg, eax
        ret 4
    }
}
__declspec(naked) static void vf_at_20c()
{
    __asm {
        mov g_call1_this, ecx
        mov eax, dword ptr [esp+4]
        mov g_call1_arg, eax
        ret 4
    }
}

int main()
{
    // Build a vtable large enough for offset 0x20c (0x210 bytes).
    static unsigned char vtbl[0x210];
    memset(vtbl, 0, sizeof(vtbl));
    *(void**)(vtbl + 0xc0)  = (void*)&vf_at_c0;
    *(void**)(vtbl + 0x20c) = (void*)&vf_at_20c;

    // Object: [0]=vtbl ptr; field @ +0x15c and +0x168.
    static unsigned char obj[0x170];
    memset(obj, 0, sizeof(obj));
    *(void**)(obj + 0x00) = (void*)vtbl;
    *(int*)(obj + 0x15c) = 0x11112222;   // field_15c
    *(int*)(obj + 0x168) = 0x33334444;   // field_168

    void* self = obj;
    // invoke with this=ecx
    __asm {
        mov ecx, self
        call OnUnhovered
    }

    int ok = 1;
    if (g_call0_this != (int)self)        { printf("FAIL call0 this\n"); ok = 0; }
    if (g_call0_arg  != 0x11112222)       { printf("FAIL call0 arg=%08x\n", g_call0_arg); ok = 0; }
    if (g_call1_this != (int)self)        { printf("FAIL call1 this\n"); ok = 0; }
    if (g_call1_arg  != 0x33334444)       { printf("FAIL call1 arg=%08x\n", g_call1_arg); ok = 0; }

    if (ok) { printf("OK_0x0055b9a0\n"); return 0; }
    return 1;
}