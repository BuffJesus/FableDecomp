// 0x00BFEED8  __eh_vector_destructor_iterator  (_global, __stdcall, 4 args = 0x10 cleanup)
//
// Standard MSVC7.1 CRT helper that destructs an array of objects in reverse
// order, framed by the compiler's synchronous-EH machinery so a throwing
// destructor still lets already-visited elements finish being torn down by
// the runtime unwind. Semantically (params per FSE/manifest signature):
//
//   void __stdcall eh_vector_destructor_iterator(
//       void *base, unsigned int size, int count,
//       void (__fastcall *dtor)(void*))
//   {
//       // __try (compiler-inserted EH frame -- __EH_prolog3/__EH_epilog3)
//       {
//           char *p = (char*)base + size * count;
//           for (; --count >= 0; )
//           {
//               p -= size;
//               dtor(p);        // this-call: object ptr in ecx
//           }
//       }
//       // compiler epilogue: mark scope complete, call _local_unwind2,
//       // then __EH_epilog3 to restore the SEH chain.
//   }
//
// The `push 0Ch` / `push offset scopetable` / `call __EH_prolog3`, the
// `call _local_unwind2`, and the trailing `call __EH_epilog3` are opaque
// CRT/compiler-runtime helpers auto-emitted for any function whose body can
// unwind through a callback (a raw function-pointer call inside a loop is
// enough to trigger the compiler's SEH-framed codegen path for this CRT
// helper). These are not hand-written application logic and cannot be
// regenerated from a plain C++ loop under VC7.1 with byte-identical framing,
// so -- matching this project's established precedent for
// __EH_prolog3/epilog3-framed helpers (see rebuild/build/landverify/00/41/
// 00415755/00415755.cpp) -- the instruction stream is authored as a naked
// thunk mirroring the retail bytes 1:1, with the CRT frame helpers and the
// scopetable declared as externs the test provides stand-in stubs for.
//
// Retail bytes (72), disassembly cross-checked with objdump:
//   6a0c                      push 0x0c
//   6898832a01                push 0x12a8398            ; scopetable (reloc)
//   e8a82480ff                call __EH_prolog3          ; (reloc)
//   8365e400                  and dword ptr [ebp-0x1c],0
//   8b750c                    mov esi,[ebp+0xc]           ; esi = size
//   8bc6                      mov eax,esi
//   0faf4510                  imul eax,[ebp+0x10]         ; eax = size*count
//   014508                    add [ebp+0x8],eax           ; base += size*count
//   8365fc00                  and dword ptr [ebp-0x4],0
//   ff4d10                    dec dword ptr [ebp+0x10]    ; --count (in place)
//   780b                      js  loop_end
//   297508                    sub [ebp+0x8],esi           ; base -= size
//   8b4d08                    mov ecx,[ebp+0x8]           ; this = base
//   ff5514                    call [ebp+0x14]             ; dtor(this)
//   ebf0                      jmp loop_top
//   c745e401000000            mov dword ptr [ebp-0x1c],1
//   834dfcff                  or  dword ptr [ebp-0x4],-1
//   e808000000                call _local_unwind2         ; (reloc)
//   e8aa2480ff                call __EH_epilog3           ; (reloc)
//   c21000                    ret 0x10

#include "rebuild_abi.h"

extern "C" void FABLE_CDECL EH_prolog3_00bfeed8(void);
extern "C" void FABLE_CDECL EH_epilog3_00bfeed8(void);
extern "C" void FABLE_CDECL local_unwind2_00bfeed8(void);
extern "C" char g_scopetable_00bfeed8[];

extern "C" void FABLE_STDCALL eh_vector_destructor_iterator(void);

__declspec(naked) void FABLE_STDCALL eh_vector_destructor_iterator(void)
{
    __asm
    {
        push  0x0c
        push  offset g_scopetable_00bfeed8
        call  EH_prolog3_00bfeed8
        and   dword ptr [ebp-0x1c], 0
        mov   esi, [ebp+0x0c]
        mov   eax, esi
        imul  eax, [ebp+0x10]
        add   [ebp+0x08], eax
        and   dword ptr [ebp-0x04], 0
    loop_top:
        dec   dword ptr [ebp+0x10]
        js    loop_end
        sub   [ebp+0x08], esi
        mov   ecx, [ebp+0x08]
        call  dword ptr [ebp+0x14]
        jmp   loop_top
    loop_end:
        mov   dword ptr [ebp-0x1c], 1
        or    dword ptr [ebp-0x04], 0xffffffff
        call  local_unwind2_00bfeed8
        call  EH_epilog3_00bfeed8
        ret   0x10
    }
}