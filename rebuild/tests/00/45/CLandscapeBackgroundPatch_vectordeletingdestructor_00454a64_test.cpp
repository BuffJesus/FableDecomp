#include <cstdio>

struct CLandscapeBackgroundPatch { void** vft; };

static int g_dtor_called = 0;
static int g_delete_called = 0;
static int g_ctor_called = 0;
static int g_new_called = 0;

void __fastcall LBP_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }
void __cdecl LBP_delete(void* p) { (void)p; g_delete_called++; }
void* __cdecl LBP_new(unsigned int sz) { (void)sz; g_new_called++; return 0; }
void __fastcall LBP_ctor(CLandscapeBackgroundPatch* self) { (void)self; g_ctor_called++; }

__declspec(naked) void* __fastcall
CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags)
{
    (void)self; (void)edx; (void)flags;
    __asm {
        push esi
        mov  esi, ecx
        call LBP_dtor
        test byte ptr [esp+8], 1
        je   short L_ret
        push esi
        call LBP_delete
        pop  ecx
    L_ret:
        mov  eax, esi
        pop  esi
        ret  4
        push 0x58
        call LBP_new
        test eax, eax
        pop  ecx
        je   short L_zero
        mov  ecx, eax
        jmp  LBP_ctor
    L_zero:
        xor  eax, eax
        ret
    }
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.vft = 0;

    g_dtor_called = 0; g_delete_called = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL path A r=%p dtor=%d del=%d\n", r, g_dtor_called, g_delete_called);
        return 1;
    }

    g_dtor_called = 0; g_delete_called = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1) {
        std::printf("FAIL path B r=%p dtor=%d del=%d\n", r, g_dtor_called, g_delete_called);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00454a64_TEST PASS\n");
    return 0;
}