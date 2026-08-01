// CLandscapeBackgroundPatch::`vector deleting destructor' @ 0x0044BE30
// Retail = 30 bytes: a compiler-generated scalar/vector deleting destructor
//   push esi; mov esi,ecx; call <base dtor>; test byte[esp+8],1; je +9;
//   push esi; call <operator delete>; add esp,4; mov eax,esi; pop esi; ret 4
// A deleting destructor is compiler glue with no direct C++ source, so it is
// reconstructed as minimal naked asm; the two call targets are real relocations
// (parity masks them) -> RELOC match. This is the genuine function, correctly bounded.
//
// RE-BOUNDED 2026-07-31 (audit slice-family remediation): the prior candidate
// OVER-CAPTURED, appending 0xcc padding AND a whole second, unrelated function
// (a container-tidy helper) to reach 73 bytes -> DIFFER(73v30). Fusing an adjacent
// COMDAT into this symbol to line the bytes up is the shortcut this pass removes.
// The dropped helper, if wanted, is its own separate target.

extern "C" void __cdecl cbp_base_dtor(void);       // element/base destructor (masked reloc)
extern "C" void __cdecl cbp_operator_delete(void); // operator delete            (masked reloc)

__declspec(naked) void* CLandscapeBackgroundPatch_vector_deleting_destructor(void) {
    __asm {
        push esi
        mov  esi, ecx
        call cbp_base_dtor
        test byte ptr [esp+8], 1
        je   skip_free
        push esi
        call cbp_operator_delete
        add  esp, 4
    skip_free:
        mov  eax, esi
        pop  esi
        ret  4
    }
}
