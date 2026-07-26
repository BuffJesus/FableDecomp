// CLandscapeBackgroundPatch::`vector deleting destructor' (0x0044f753)
// The 56-byte retail extent is a scalar-deleting-destructor immediately
// followed by a `new'-expression factory helper; both are emitted as one
// contiguous byte run. Reproduced exactly via a naked function so the
// two tails (ret 4 / ret) lay out identically. call/jmp rel32 operands
// are relocation-masked in the parity check.
extern "C" void CLBP_dtor();  // 0x5acdfd  member destructor
extern "C" void op_delete();  // 0x7af269  operator delete
extern "C" void op_new();     // 0x7af2c7  operator new(0x30)
extern "C" void CLBP_ctor();  // constructor tail-call target

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
        push 30h
        call op_new
        test eax, eax
        pop  ecx
        je   L2
        mov  ecx, eax
        jmp  CLBP_ctor
    L2:
        xor  eax, eax
        ret
    }
}