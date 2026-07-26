#include <cstdio>
struct CLandscapeBackgroundPatch { void* vtbl; };
static int g_dtor = 0;
static int g_del = 0;
void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self){ (void)self; g_dtor++; }
void __fastcall CLandscapeBackgroundPatch_ctor(CLandscapeBackgroundPatch* self){ (void)self; }
void __cdecl op_delete(void* p){ (void)p; g_del++; }
void* __cdecl op_new(unsigned int n){ (void)n; return 0; }
__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch*, int, unsigned int)
{
    __asm {
        push esi
        mov  esi, ecx
        call CLandscapeBackgroundPatch_dtor
        test byte ptr [esp+8], 1
        je   L1
        push esi
        call op_delete
        pop  ecx
    L1:
        mov  eax, esi
        pop  esi
        ret  4
        push 0x58
        call op_new
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
int main(){
    CLandscapeBackgroundPatch obj; obj.vtbl=0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor != 1 || g_del != 0) { std::printf("FAIL a\n"); return 1; }
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor != 2 || g_del != 1) { std::printf("FAIL b\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00454a00_TEST PASS\n");
    return 0;
}