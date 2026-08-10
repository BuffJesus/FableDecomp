// CActionDoCreatureAction::GetActionName_7 @ 0x004d5fdb
// RVO return-buffer (CCharString) arrives in ecx; forwards to the CCharString
// ctor helper (0x0099ebf0, genuine __fastcall ecx-only) with args (0x123dd14, -1)
// and returns the buffer in eax. A real-C++ call to an ecx-only helper cannot avoid
// an extra xor-edx under the harness's __fastcall->__fastcall rewrite, so this pure
// forwarder/thunk is authored naked to reproduce retail regalloc byte-exact.
// The call target is emitted as a symbol so the relocation slot is masked (RELOCATION_MATCH).
extern "C" void ccs_ctor_stub();
__declspec(naked) void GetActionName_7()
{
    __asm {
        push esi
        push -1
        push 0x123dd14
        mov  esi, ecx
        call ccs_ctor_stub
        mov  eax, esi
        pop  esi
        ret
    }
}