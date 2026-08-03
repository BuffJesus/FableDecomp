// vector_deleting_destructor for CLandscapeBackgroundPatch (00454a32)
// Real retail function is 28 bytes: the scalar deleting destructor.
// The prior 50-byte capture fused an adjacent creation helper; dropped.
extern "C" void CLandscapeBackgroundPatch_inner_dtor(void);
extern "C" void CLandscapeBackgroundPatch_op_delete(void);

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
    }
}