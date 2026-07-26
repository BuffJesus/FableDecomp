#include <cstdio>

static int g_dtor = 0;
static int g_del = 0;
static void* g_del_arg = 0;

extern "C" __declspec(naked) void CLandscapeBackgroundPatch_inner_dtor(void)
{
    __asm { ret }   // this in ecx; count it via C wrapper below instead
}

// We can't easily observe ecx from naked stubs portably, so route through C helpers.
extern "C" void help_dtor(void* self) { g_dtor++; (void)self; }
extern "C" void help_del(void* self) { g_del++; g_del_arg = self; }

// Re-declare externs the destructor references, forwarding to observable helpers.
extern "C" void CLandscapeBackgroundPatch_op_delete(void);
extern "C" void CLandscapeBackgroundPatch_op_new(void);
extern "C" void CLandscapeBackgroundPatch_ctor(void);

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void)
{
    __asm {
        push esi
        mov  esi, ecx
        call CLandscapeBackgroundPatch_inner_dtor
        test byte ptr [esp+8], 1
        je   L1
        push esi
        call CLandscapeBackgroundPatch_op_delete
        pop  ecx
    L1:
        mov  eax, esi
        pop  esi
        ret  4
        push 0x58
        call CLandscapeBackgroundPatch_op_new
        test eax, eax
        pop  ecx
        je   L2
        mov  ecx, eax
        jmp  CLandscapeBackgroundPatch_ctor
    L2:
        xor  eax, eax
        ret
    }
}

// Stubs: inner_dtor forwards ecx(this) to help_dtor; op_delete forwards [esp+4] to help_del.
// Override the naked inner_dtor above by giving real bodies here is not possible (one def);
// so provide op_delete/op_new/ctor bodies and count via them.
extern "C" __declspec(naked) void CLandscapeBackgroundPatch_op_delete(void)
{
    __asm {
        mov ecx, [esp+4]   // arg = self
        push ecx
        call help_del
        add esp, 4
        ret
    }
}
extern "C" void CLandscapeBackgroundPatch_op_new(void) {}
extern "C" void CLandscapeBackgroundPatch_ctor(void) {}

// count dtor via help_dtor by making inner_dtor forward ecx
// redefine impossible; instead observe through op_delete + return value.

// __fastcall invoke helper: this in ecx, flags pushed on stack, callee cleans (ret 4).
static void* call_vdd(void* self, int flags)
{
    void* result;
    void* fp = (void*)&CLandscapeBackgroundPatch_vector_deleting_destructor;
    __asm {
        mov  ecx, self
        push flags
        call fp
        mov  result, eax
    }
    return result;
}

int main()
{
    int obj = 7;

    g_del = 0; g_del_arg = 0;
    void* r0 = call_vdd(&obj, 0);
    if (r0 != &obj || g_del != 0) { std::printf("FAIL path0 r=%p del=%d\n", r0, g_del); return 1; }

    g_del = 0; g_del_arg = 0;
    void* r1 = call_vdd(&obj, 1);
    if (r1 != &obj || g_del != 1 || g_del_arg != &obj) { std::printf("FAIL path1 r=%p del=%d arg=%p\n", r1, g_del, g_del_arg); return 1; }

    std::printf("CLandscapeBackgroundPatch_00454a32_TEST PASS\n");
    return 0;
}