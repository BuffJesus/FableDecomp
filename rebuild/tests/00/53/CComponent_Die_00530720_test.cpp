#include <cstdio>

// ---- observation state ----
static void* g_vcall_this   = 0;
static int   g_vcall_arg    = 0;
static void* g_tail_this    = 0;

// __fastcall virtual at slot +0xC0: this in ecx, one stack arg (int).
// Callee cleans the arg (__fastcall/stdcall-style: ret 4).
static void __declspec(naked) VFn_0xC0(void)
{
    __asm
    {
        mov  g_vcall_this, ecx
        mov  eax, [esp+4]        // the pushed arg
        mov  g_vcall_arg, eax
        ret  4
    }
}

// Tail target 0x5303f0 stub: records this (ecx), then returns to Die's caller.
extern "C" __declspec(naked) void Die_TailTarget_0x5303f0(void)
{
    __asm
    {
        mov  g_tail_this, ecx
        ret
    }
}

// ---- function under test (byte-identical to source_cpp) ----
__declspec(naked) void Die(void)
{
    __asm
    {
        push esi
        mov  esi, ecx
        mov  eax, dword ptr [esi]
        push 2
        call dword ptr [eax+0x0c0]
        mov  ecx, esi
        pop  esi
        jmp  Die_TailTarget_0x5303f0
    }
}

int main(void)
{
    // Build a fake object: [0] = vtable ptr. vtable large enough for slot 0xC0.
    void* vtable[64];               // 64*4 = 0x100 bytes, slot 0xC0/4 = 48 in range
    for (int i = 0; i < 64; ++i) vtable[i] = 0;
    vtable[0xC0 / 4] = (void*)&VFn_0xC0;

    void* obj[4];
    obj[0] = (void*)vtable;         // obj+0 -> vtable

    void* this_ptr = (void*)obj;

    g_vcall_this = 0; g_vcall_arg = -1; g_tail_this = 0;

    __asm
    {
        mov  ecx, this_ptr
        call Die
    }

    bool ok = (g_vcall_this == this_ptr)
           && (g_vcall_arg  == 2)
           && (g_tail_this  == this_ptr);

    if (ok)
    {
        std::printf("OK_0x00530720\n");
        return 0;
    }
    std::printf("FAIL_0x00530720 vthis=%p varg=%d tail=%p want=%p\n",
                g_vcall_this, g_vcall_arg, g_tail_this, this_ptr);
    return 1;
}