#include "rebuild_abi.h"

// 0x00BFEE7A  __ArrayUnwind  (_global, __stdcall, 4 args = 0x10 cleanup, 93 bytes)
//
// MSVC7.1 CRT helper __ArrayUnwind (paired with its except-filter,
// ArrayUnwindFilter, which the retail compiler emitted immediately after it
// in the same translation unit -- both routines are contained in this
// 93-byte byte range per the manifest/coverage boundary at 0x00BFEE7A..
// 0x00BFEED6, with the next distinct function, eh_vector_destructor_iterator,
// starting at 0x00BFEED8).
//
// Semantically (params per FSE/manifest signature):
//
//   void __stdcall __ArrayUnwind(void *base, unsigned int size,
//                                 int count, void (__fastcall *dtor)(void*))
//   {
//       __try
//       {
//           for (; --count >= 0; )
//           {
//               base = (char*)base - size;
//               dtor(base);          // this-call: object ptr in ecx
//           }
//       }
//       __except (ArrayUnwindFilter(GetExceptionInformation()))
//       {
//           // handled entirely inside the compiler-generated filter/handler
//           // glue below; ArrayUnwindFilter tests the C++ EH exception code
//           // (0xE06D7363) and, if it matches, continues the frame-based
//           // stack unwind (call to the CRT unwind continuation at
//           // 0x00BFEFAA) instead of re-raising.
//       }
//   }
//
// This uses the compact (non-"3") __EH_prolog/__EH_epilog CRT entry points
// (same family already landed for 0047d5a3::PeekGuiDef), NOT the __EH_prolog3
// / scopetable-table variant used by the neighbouring eh_vector_destructor_
// iterator. The filter function occupies the tail of the same byte range and
// is invoked by the CRT's SEH dispatcher (_except_handler3-style) sharing
// the protected frame's ebp -- it is not a normal call from inside the body.
//
// Retail disassembly (call rel32 / pushed absolute operands relocation-masked
// in parity):
//
//   push 0x14                     ; EH record size
//   push 0x12a8388                ; funcinfo/scopetable (reloc)
//   call __EH_prolog               ; establish SEH frame, ebp = &arg1
//   and  dword ptr [ebp-4], 0      ; EH state = 0 (inside try)
// loop_top:
//   dec  dword ptr [ebp+0x10]      ; --count
//   js   loop_end
//   mov  ecx, [ebp+8]              ; ecx = base
//   sub  ecx, [ebp+0xc]            ; ecx -= size
//   mov  [ebp+8], ecx              ; base -= size
//   call dword ptr [ebp+0x14]      ; dtor(base)   (this = base, via ecx)
//   jmp  loop_top
// loop_end:
//   or   dword ptr [ebp-4], 0xffffffff  ; EH state = -1 (done)
//   call __EH_epilog                ; tear down SEH frame
//   ret  0x10
//
// -- ArrayUnwindFilter tail (called by the SEH dispatcher, ebp shared with
//    the frame above; ebp-0x14 is the exception-pointers slot the CRT's
//    _EH_prolog convention reserves in the protected function's frame):
//
//   mov  eax, [ebp-0x14]           ; eax = ExceptionPointers
//   mov  [ebp-0x1c], eax
//   mov  eax, [ebp-0x1c]
//   mov  eax, [eax]                ; eax = ExceptionRecord
//   mov  [ebp-0x20], eax
//   mov  eax, [ebp-0x20]
//   cmp  dword ptr [eax], 0xe06d7363   ; ExceptionCode == C++ EH magic?
//   je   is_cxx_eh
//   mov  dword ptr [ebp-0x24], 0       ; filter result = 0 (continue search)
//   mov  eax, [ebp-0x24]
//   ret
// resume_unwind:                       ; unreachable fallthrough target of a
//                                       ; dispatcher-driven re-entry (dead
//                                       ; retail code path in this build)
//   mov  esp, [ebp-0x18]
//   jmp  loop_end
// is_cxx_eh:
//   call 0x00bfefaa                    ; CRT unwind continuation (external)
//
// Because this filter is invoked out-of-band by the SEH dispatcher (its own
// calling convention is fixed by the OS/CRT contract, not by ordinary C++
// control flow reachable from __ArrayUnwind's body) and the compact
// __EH_prolog/__EH_epilog entry points are opaque CRT internals (same
// precedent as 0047d5a3/00bfeed8), the whole 93-byte range is authored as a
// single naked thunk mirroring the retail bytes 1:1, with the CRT frame
// helpers and the unwind continuation declared as externs the test provides
// stand-ins for.

extern "C" void FABLE_CDECL EH_prolog_00bfee7a(void);
extern "C" void FABLE_CDECL EH_epilog_00bfee7a(void);
extern "C" void FABLE_CDECL UnwindContinue_00bfee7a(void);

// Relocation-masked funcinfo/scopetable pointer pushed as the EH argument.
extern "C" char g_funcinfo_00bfee7a[];

extern "C" void FABLE_STDCALL ArrayUnwind_00bfee7a(void);

__declspec(naked) void FABLE_STDCALL ArrayUnwind_00bfee7a(void)
{
    __asm
    {
        push  0x14
        push  offset g_funcinfo_00bfee7a
        call  EH_prolog_00bfee7a
        and   dword ptr [ebp-4], 0
    loop_top:
        dec   dword ptr [ebp+0x10]
        js    loop_end
        mov   ecx, dword ptr [ebp+8]
        sub   ecx, dword ptr [ebp+0xc]
        mov   dword ptr [ebp+8], ecx
        call  dword ptr [ebp+0x14]
        jmp   loop_top
    loop_end:
        or    dword ptr [ebp-4], 0xffffffff
        call  EH_epilog_00bfee7a
        ret   0x10

        ; -- ArrayUnwindFilter tail (same byte range, dispatcher-invoked) --
        mov   eax, dword ptr [ebp-0x14]
        mov   dword ptr [ebp-0x1c], eax
        mov   eax, dword ptr [ebp-0x1c]
        mov   eax, dword ptr [eax]
        mov   dword ptr [ebp-0x20], eax
        mov   eax, dword ptr [ebp-0x20]
        cmp   dword ptr [eax], 0e06d7363h
        je    is_cxx_eh
        mov   dword ptr [ebp-0x24], 0
        mov   eax, dword ptr [ebp-0x24]
        ret
    resume_unwind:
        mov   esp, dword ptr [ebp-0x18]
        jmp   loop_end
    is_cxx_eh:
        call  UnwindContinue_00bfee7a
    }
}