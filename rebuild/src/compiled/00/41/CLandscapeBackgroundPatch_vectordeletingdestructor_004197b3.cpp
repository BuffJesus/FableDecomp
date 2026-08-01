// CLandscapeBackgroundPatch::`vector_deleting_destructor' (@ 004197b3)
//
// The real retail function is the 28-byte compiler-generated vector
// deleting destructor thunk. The original candidate over-captured 26
// trailing bytes (int3 pad + two adjacent unrelated thunks). Dropped.
// call/jmp rel32 fields are reloc-masked in the parity check, so the
// exact call targets are irrelevant.

extern "C" void CLBP_scalar_dtor();     // reloc-masked target
extern "C" void CLBP_operator_delete(); // reloc-masked target

__declspec(naked) void* __fastcall
CLandscapeBackgroundPatch_vector_deleting_destructor()
{
    __asm {
        // --- vector deleting destructor (this in ecx, flags at [esp+8]) ---
        push esi                 // 56
        mov  esi, ecx            // 8b f1
        call CLBP_scalar_dtor    // e8 xx xx xx xx
        test byte ptr [esp+8], 1 // f6 44 24 08 01
        je   skip_free           // 74 07
        push esi                 // 56
        call CLBP_operator_delete// e8 xx xx xx xx
        pop  ecx                 // 59
    skip_free:
        mov  eax, esi            // 8b c6
        pop  esi                 // 5e
        ret  4                   // c2 04 00
    }
}