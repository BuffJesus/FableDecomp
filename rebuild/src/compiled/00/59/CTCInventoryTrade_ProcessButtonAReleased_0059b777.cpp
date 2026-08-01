// CTCInventoryTrade::ProcessButtonAReleased @ 0x0059B777
// Retail = 25 bytes: __fastcall (this=ecx). If field +0x1c == 1, dispatch vtable
// slot [+0x110]; then tail-jmp to the button-A helper with `this` preserved:
//   push esi; mov esi,ecx; cmp [esi+0x1c],1; jne +8; mov eax,[esi];
//   call [eax+0x110]; mov ecx,esi; pop esi; jmp <tail>
// The tail-call jmp (e9 rel32) is a compiler tail-call opt with no clean C++ form,
// so it is reconstructed as minimal naked asm; the jmp target is a masked relocation
// -> RELOC. The function ends AT that jmp (byte 25).
//
// RE-BOUNDED 2026-07-31 (audit slice-family remediation): the prior candidate
// OVER-CAPTURED, appending TWO further jmp thunks (0x005bdd8c / 0x005bddb8) to reach
// 35 bytes -> DIFFER(35v25). Those are adjacent unrelated thunks, not part of this
// function. This is now JUST the real function, correctly bounded.
extern "C" void ProcessButtonAReleased_tail(void);   // 0x005BC65C  button-A helper (tail call)

__declspec(naked) void ProcessButtonAReleased()
{
    __asm {
        push esi
        mov  esi, ecx
        cmp  dword ptr [esi+0x1c], 1
        jne  skip
        mov  eax, dword ptr [esi]
        call dword ptr [eax+0x110]
    skip:
        mov  ecx, esi
        pop  esi
        jmp  ProcessButtonAReleased_tail
    }
}
