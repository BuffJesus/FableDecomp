// vector_deleting_destructor for CLandscapeBackgroundPatch (00454a32)
// Retail is two adjacent MSVC helpers glued under one 50-byte symbol:
//   [0x00] scalar deleting destructor(this, flags)
//   [0x1c] creation helper: operator new(0x58) + constructor tail-call
// Emitted verbatim so the single symbol is byte-exact (call/jmp rel32 reloc-masked).
extern "C" void CLandscapeBackgroundPatch_inner_dtor(void);
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