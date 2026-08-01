// CCarrySlotDef::GetSizeofClass @ 0x0044C1F6
// Retail = 4 bytes (6a 38 58 c3 = push 0x38; pop eax; ret) -> returns sizeof = 0x38.
// PDB: ?GetSizeofClass@CCarrySlotDef@@UAEKXZ  (public virtual __thiscall unsigned long, no args).
//
// RE-BOUNDED 2026-07-31 (audit slice-family remediation): the prior candidate was a
// 29-byte __declspec(naked) blob that OVER-CAPTURED this 4-byte function together with
// two unrelated adjacent functions (a field-clear and a ctor/factory), giving DIFFER(29v4).
// This is now JUST the real function, as faithful C++.
//
// FLAG: retail emitted the `push imm8; pop eax` size idiom, so the correct per-function
// flag is /O2 /Os (size-opt). `return 0x38` then yields EXACT 6a3858c3 (verified). Under
// plain /O2 it emits `mov eax,0x38` (b8 38 00 00 00 c3, 6 bytes) -> DIFFER(6v4). The vtable
// GetSizeofClass thunk class is size-optimized; catalog_parity_audit.py includes an /Os variant.
struct CCarrySlotDef { unsigned long _first; };

unsigned long __fastcall CCarrySlotDef_GetSizeofClass(CCarrySlotDef* self)
{
    (void)self;
    return 0x38;
}
