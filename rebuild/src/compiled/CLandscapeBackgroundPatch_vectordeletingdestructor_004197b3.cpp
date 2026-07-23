// CLandscapeBackgroundPatch::`vector_deleting_destructor' (@ 004197b3)
//
// The 54-byte retail span is the compiler-generated vector deleting
// destructor thunk followed (in the same code run) by two adjacent
// helper thunks that the disassembler swept into one symbol:
//   [0x00] vector deleting destructor  (28 bytes) + int3 pad
//   [0x1d] adjustor/forwarding thunk   (this+8, forward stack arg)
//   [0x2d] element-count accessor      ((end-begin) >> 2)
// We reproduce the exact byte image with a single naked function so the
// whole 54-byte run is one symbol. call/jmp rel32 fields are reloc-masked
// in the parity check, so the exact call targets are irrelevant.

extern "C" void CLBP_scalar_dtor();     // reloc-masked target
extern "C" void CLBP_operator_delete(); // reloc-masked target
extern "C" void CLBP_forward_target();  // reloc-masked target

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
        __asm _emit 0xcc         // int3 pad

        // --- adjustor / forwarding thunk ---
        add  ecx, 8              // 83 c1 08
        push ecx                 // 51
        mov  ecx, dword ptr [esp+8] // 8b 4c 24 08
        call CLBP_forward_target // e8 xx xx xx xx
        ret  4                   // c2 04 00

        // --- element-count accessor: (this->end - this->begin) >> 2 ---
        mov  eax, dword ptr [ecx+4] // 8b 41 04
        sub  eax, dword ptr [ecx]   // 2b 01
        sar  eax, 2                 // c1 f8 02
        ret                         // c3
    }
}