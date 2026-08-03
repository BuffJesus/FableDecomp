// CLandscapeBackgroundPatch::`vector deleting destructor' @ 0x0044FE5F
// Retail = 34 bytes: a scalar/vector deleting destructor (stores vtable, runs base
// dtor, conditionally frees) -- same shape as the 0044F90F sibling, different call
// targets. Compiler glue with no direct C++ source -> minimal naked asm; the vtable
// global + two call targets are relocations (parity masks them) -> RELOC.
//
// RE-BOUNDED 2026-07-31 (audit slice-family remediation): the prior candidate
// OVER-CAPTURED, appending an adjacent allocating-creator thunk to reach 70 bytes
// -> DIFFER(70v34). This is now JUST the real function, correctly bounded.
struct CLandscapeBackgroundPatch { void* vptr; };

extern "C" void __cdecl CLandscapeBackgroundPatch_dtor(void);   // 0x5ac6f1  base destructor
extern "C" void __cdecl CLandscapeBackgroundPatch_delete(void); // 0x7aeb5d  operator delete
extern int g_vtblA;   // 0x1230ba0  vtable

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self)
{
    __asm {
        push    esi
        mov     esi, ecx
        mov     dword ptr [esi], offset g_vtblA
        call    CLandscapeBackgroundPatch_dtor
        test    byte ptr [esp+8], 1
        je      L_skip
        push    esi
        call    CLandscapeBackgroundPatch_delete
        pop     ecx
    L_skip:
        mov     eax, esi
        pop     esi
        ret     4
    }
}
