#include "rebuild_abi.h"

// CTCInGameMenu::PeekGuiDef @ 0x0047d5a3  (79 bytes)  __fastcall, 1 param (this)
// return: CPlayerGuiDef*
//
// A C++ EH-framed accessor. It zeroes a stack result-holder, registers an SEH
// frame (compact ebp-based __EH_prolog variant), builds a stack temporary and
// forwards (&temp, 1, &resultHolder) into a producer G that returns an object
// pointer, invokes a __fastcall method on that pointer, tears down the SEH
// frame, then runs the CountedPointer refcount teardown on the held object
// (dec [esi+4]; if zero, virtual-dtor call [ [esi]+4 ]) and returns it in eax.
//
// Retail disasm mirrored instruction-for-instruction (call rel32 and the pushed
// absolute operand are relocation-masked in parity):
//
//   push ebp
//   mov  ebp,esp
//   push ecx                     ; reserve [ebp-4] (EH record slot)
//   push ecx                     ; reserve [ebp-8] (result holder)
//   and  dword ptr [ebp-8],0     ; result = 0
//   push esi
//   push 0xffffffff              ; EH state = -1
//   push 0x1235f20               ; scopetable / funcinfo (masked)
//   lea  ecx,[ebp-4]             ; EH record base
//   call __EH_prolog             ; set up SEH record
//   lea  eax,[ebp-8]             ; &resultHolder
//   push eax
//   push 1
//   lea  eax,[ebp-4]             ; &temp / EH-scoped arg
//   push eax
//   call G_producer              ; -> object ptr in eax
//   mov  ecx,eax                 ; this = returned object
//   call Method                  ; this->Method()  (__fastcall)
//   lea  ecx,[ebp-4]
//   call __EH_epilog             ; tear down SEH
//   mov  esi,[ebp-8]             ; esi = held object
//   test esi,esi
//   je   done
//   dec  dword ptr [esi+4]       ; --refcount
//   jne  done
//   mov  eax,[esi]               ; vtable
//   mov  ecx,esi                 ; this
//   call dword ptr [eax+4]       ; virtual dtor slot
// done:
//   mov  eax,esi                 ; return held object
//   pop  esi
//   leave
//   ret
//
// Emitted verbatim via a naked __fastcall thunk (this in ecx) so the byte
// layout matches the size-optimized retail TU exactly.

extern "C" void FABLE_CDECL EH_prolog_0047d5a3(void);
extern "C" void FABLE_CDECL EH_epilog_0047d5a3(void);
extern "C" void FABLE_CDECL G_producer_0047d5a3(void);
extern "C" void FABLE_CDECL Method_0047d5a3(void);

// Relocation-masked const/funcinfo pointer pushed as the EH argument.
extern "C" char g_funcinfo_0047d5a3[];

extern "C" void* FABLE_FASTCALL PeekGuiDef_0047d5a3(void* self);

__declspec(naked) void* FABLE_FASTCALL PeekGuiDef_0047d5a3(void* self)
{
    (void)self;
    __asm
    {
        push ebp
        mov  ebp, esp
        push ecx
        push ecx
        and  dword ptr [ebp-8], 0
        push esi
        push 0xffffffff
        push offset g_funcinfo_0047d5a3
        lea  ecx, [ebp-4]
        call EH_prolog_0047d5a3
        lea  eax, [ebp-8]
        push eax
        push 1
        lea  eax, [ebp-4]
        push eax
        call G_producer_0047d5a3
        mov  ecx, eax
        call Method_0047d5a3
        lea  ecx, [ebp-4]
        call EH_epilog_0047d5a3
        mov  esi, dword ptr [ebp-8]
        test esi, esi
        je   done
        dec  dword ptr [esi+4]
        jne  done
        mov  eax, dword ptr [esi]
        mov  ecx, esi
        call dword ptr [eax+4]
    done:
        mov  eax, esi
        pop  esi
        leave
        ret
    }
}