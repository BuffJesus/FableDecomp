// CLandscapeBackgroundPatch::`vector deleting destructor' @ 0x0044F90F
// Retail = 34 bytes: a scalar/vector deleting destructor that stores the vtable,
// runs the base dtor, and conditionally frees:
//   push esi; mov esi,ecx; mov [esi],<vftable>; call <base dtor>;
//   test byte[esp+8],1; je +7; push esi; call <operator delete>; pop ecx;
//   mov eax,esi; pop esi; ret 4
// Compiler glue with no direct C++ source -> reconstructed as minimal naked asm;
// the vtable global + two call targets are relocations (parity masks them) -> RELOC.
//
// RE-BOUNDED 2026-07-31 (audit slice-family remediation): the prior candidate
// OVER-CAPTURED, appending an adjacent allocating-constructor thunk to reach 56
// bytes -> DIFFER(56v34). This is now JUST the real function, correctly bounded.
struct CLandscapeBackgroundPatch { void* vptr; };
extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern void __cdecl op_delete(void* p);
extern void* const CLandscapeBackgroundPatch_vftable;

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, unsigned int flags)
{
    (void)self; (void)flags;
    __asm {
        push    esi
        mov     esi, ecx
        mov     dword ptr [esi], offset CLandscapeBackgroundPatch_vftable
        call    CLandscapeBackgroundPatch_dtor
        test    byte ptr [esp+8], 1
        je      skip_delete
        push    esi
        call    op_delete
        pop     ecx
    skip_delete:
        mov     eax, esi
        pop     esi
        ret     4
    }
}
