// CLandscapeBackgroundPatch::`vector deleting destructor' (0x0044f753)
// The real 34-byte retail function is a scalar/vector-deleting destructor
// terminating at `ret 4`. The prior 56-byte capture fused an adjacent
// `new'-expression factory helper; that tail is dropped here.
// call rel32 operands are relocation-masked in the parity check.
extern "C" void CLBP_dtor();  // 0x5acdfd  member destructor
extern "C" void op_delete();  // 0x7af269  operator delete

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor()
{
    __asm {
        push esi
        mov  esi, ecx
        mov  dword ptr [esi], 01230BA0h
        call CLBP_dtor
        test byte ptr [esp+8], 1
        je   L1
        push esi
        call op_delete
        pop  ecx
    L1:
        mov  eax, esi
        pop  esi
        ret  4
    }
}
