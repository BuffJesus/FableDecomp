#include <cstdio>

// Stubs
static int g_dtor_calls = 0;
static int g_del_calls = 0;
static void* g_vftable[4] = {0,0,0,0};

struct CLandscapeBackgroundPatch { void* vtbl; };

extern "C" void __fastcall CInner_dtor(void* self) { (void)self; g_dtor_calls++; }
extern "C" void __cdecl engine_delete(void* p) { (void)p; g_del_calls++; }
extern "C" void* __cdecl engine_new(unsigned int sz) { (void)sz; return 0; }
extern "C" void __fastcall CLandscapeBackgroundPatch_ctor(void* self) { (void)self; }
void* const CLandscapeBackgroundPatch_vftable[4] = {0,0,0,0};

// Copy of the reconstructed function body
__declspec(naked) void* __fastcall the_dtor(CLandscapeBackgroundPatch* self, unsigned int flags)
{
    __asm {
        push esi
        mov  esi, ecx
        mov  dword ptr [esi], offset CLandscapeBackgroundPatch_vftable
        call CInner_dtor
        test byte ptr [esp+8], 1
        je   L1
        push esi
        call engine_delete
        pop  ecx
    L1:
        mov  eax, esi
        pop  esi
        ret  4
        push 0x38
        call engine_new
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

// Call the __fastcall-style thunk: this in ecx, flags on stack, callee does ret 4.
static void* call_dtor(CLandscapeBackgroundPatch* self, unsigned int flags)
{
    void* ret;
    void* fn = (void*)&the_dtor;
    __asm {
        push flags        // flags -> [esp+8] after the callee's own push
        mov  ecx, self    // this
        call fn           // callee returns self in eax, cleans its 4-byte arg (ret 4)
        mov  ret, eax
    }
    return ret;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.vtbl = 0;

    // flags=0: dtor runs, no delete, vtbl set, returns self
    void* r = call_dtor(&obj, 0);
    if (r != &obj) { std::printf("FAIL: return not self\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL: dtor not called\n"); return 1; }
    if (g_del_calls != 0) { std::printf("FAIL: delete called w/o flag\n"); return 1; }
    if (obj.vtbl != (void*)CLandscapeBackgroundPatch_vftable) { std::printf("FAIL: vtbl not set\n"); return 1; }

    // flags=1: dtor + delete
    void* r2 = call_dtor(&obj, 1);
    if (r2 != &obj) { std::printf("FAIL: return2 not self\n"); return 1; }
    if (g_dtor_calls != 2) { std::printf("FAIL: dtor2\n"); return 1; }
    if (g_del_calls != 1) { std::printf("FAIL: delete not called\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_0044f71b_TEST PASS\n");
    return 0;
}