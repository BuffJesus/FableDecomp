#include <stdio.h>

// ---- function-under-test (same body as source_cpp) ----
__declspec(naked) void ConstructItemDescription(void *arg0, void *arg1)
{
    (void)arg0; (void)arg1;
    __asm
    {
        mov eax, dword ptr [ecx]
        push 0
        push dword ptr [esp+0x0c]
        push dword ptr [esp+0x0c]
        call dword ptr [eax+0x138]
        ret 8
    }
}

// ---- captured args from the virtual dispatch ----
static void*        g_this = 0;
static void*        g_a0   = 0;
static void*        g_a1   = 0;
static int          g_a2   = -1;
static int          g_hits = 0;

// virtual callee: __fastcall with 3 stack args -> this in ecx, cleans 12 bytes.
__declspec(naked) void virt_stub()
{
    __asm
    {
        mov g_this, ecx
        mov eax, dword ptr [esp+4]   // arg0
        mov g_a0, eax
        mov eax, dword ptr [esp+8]   // arg1
        mov g_a1, eax
        mov eax, dword ptr [esp+0x0c] // arg2 (the pushed 0)
        mov g_a2, eax
        inc g_hits
        ret 0x0c
    }
}

// object with a vtable; slot 0x138 (index 78) -> virt_stub
static void* g_vtbl[80];
// object: first field is the vtable pointer, so [obj] == &g_vtbl.
static void* g_obj[4];

int main()
{
    for (int i = 0; i < 80; ++i) g_vtbl[i] = 0;
    g_vtbl[78] = (void*)&virt_stub;      // 78*4 = 0x138

    g_obj[0] = (void*)&g_vtbl;            // vtable pointer at object+0
    void* obj = (void*)&g_obj;            // [obj] == g_obj[0] == &g_vtbl
    void* A = (void*)0xAAAA0001;
    void* B = (void*)0xBBBB0002;

    // invoke: this=ecx=obj, two stack args (A,B), callee does ret 8.
    __asm
    {
        push B
        push A
        mov  ecx, obj
        call ConstructItemDescription
    }

    if (g_hits != 1)                 { printf("FAIL hits=%d\n", g_hits); return 1; }
    if (g_this != obj)               { printf("FAIL this=%p\n", g_this); return 1; }
    if (g_a0   != A)                 { printf("FAIL a0=%p\n", g_a0);     return 1; }
    if (g_a1   != B)                 { printf("FAIL a1=%p\n", g_a1);     return 1; }
    if (g_a2   != 0)                 { printf("FAIL a2=%d\n", g_a2);     return 1; }

    printf("OK_0x005819dd this=%p a0=%p a1=%p a2=%d\n", g_this, g_a0, g_a1, g_a2);
    return 0;
}