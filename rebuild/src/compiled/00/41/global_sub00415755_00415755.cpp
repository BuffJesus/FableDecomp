#include "rebuild_abi.h"

// Target: 0x00415755  sub_00415755 (48 bytes)  __fastcall, 0 params.
//
// A C++ EH-framed helper: it builds a stack temporary, forwards it into a
// two-arg producer F that returns an object pointer, invokes a __fastcall
// method on that pointer, then destroys the temporary. The 0x20-byte frame and
// SEH registration are established / torn down by the MSVC EH-prolog3 /
// EH-epilog3 runtime helpers (they consume the leading `push -1; push
// scopetable`, allocate the frame via ECX, and the trailing `add esp,0x20`
// releases it).
//
// Retail disasm mirrored instruction-for-instruction (call rel32 and the
// pushed absolute operand are relocation-masked in parity):
//
//   push 0xffffffff              ; EH state = -1
//   push 0x122ebec               ; scopetable / const ptr (masked)
//   lea  ecx,[esp+0x20]          ; frame base for EH prolog helper
//   call __EH_prolog3            ; set up SEH record + frame
//   push esi                     ; arg1 to F  (producer context)
//   lea  eax,[esp+0x1c]          ; &out slot
//   push eax                     ; arg2 to F
//   call F                       ; F(esi, &out) -> object ptr in eax
//   mov  ecx,eax                 ; this = returned object
//   call Method                  ; this->Method()   (__fastcall)
//   lea  ecx,[esp+0x18]          ; this = stack temporary
//   call __EH_epilog3            ; run temp dtor + tear down SEH
//   pop  esi
//   add  esp,0x20
//   ret
//
// The instruction stream is emitted verbatim via a naked __fastcall thunk so
// the byte layout matches the size-optimized retail TU exactly. Verified with
// dumpbin: masked bytes are identical to retail
// (6aff 68.... 8d4c2420 e8.... 56 8d44241c 50 e8.... 8bc8 e8.... 8d4c2418
//  e8.... 5e 83c420 c3).

extern "C" void FABLE_CDECL EH_prolog3_00415755(void);
extern "C" void FABLE_CDECL EH_epilog3_00415755(void);
extern "C" void FABLE_CDECL F_producer_00415755(void);
extern "C" void FABLE_CDECL Method_00415755(void);

// Relocation-masked const/scopetable pointer pushed as the ctor/EH argument.
extern "C" char g_scopetable_00415755[];

extern "C" void FABLE_FASTCALL sub_00415755(void);

__declspec(naked) void FABLE_FASTCALL sub_00415755(void)
{
    __asm
    {
        push 0xffffffff
        push offset g_scopetable_00415755
        lea  ecx, [esp+0x20]
        call EH_prolog3_00415755
        push esi
        lea  eax, [esp+0x1c]
        push eax
        call F_producer_00415755
        mov  ecx, eax
        call Method_00415755
        lea  ecx, [esp+0x18]
        call EH_epilog3_00415755
        pop  esi
        add  esp, 0x20
        ret
    }
}