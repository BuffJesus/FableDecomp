// CHeroMarriageDef::GetSizeofClass @ 0x0044C2E2
// Retail = 6 bytes (b8 c8 00 00 00 c3 = mov eax,0xC8; ret) -> returns sizeof = 0xC8.
// PDB: ?GetSizeofClass@CHeroMarriageDef@@UAEKXZ  (public virtual __thiscall unsigned long, no args).
//
// RE-BOUNDED 2026-07-31 (audit slice-family remediation): the prior candidate was a
// 31-byte __declspec(naked) blob that OVER-CAPTURED this 6-byte function together with an
// adjacent operator-new factory thunk (its own comment admitted the fusion), giving
// DIFFER(31v6). Fusing two functions to force a byte region to line up is exactly the kind
// of shortcut this pass removes. This is now JUST the real function, as faithful C++.
// It emits the exact 6 retail bytes under plain /O2 (mov-imm32 form; no /Os needed).
struct CHeroMarriageDef { unsigned long _first; };

unsigned long __fastcall CHeroMarriageDef_GetSizeofClass(CHeroMarriageDef* self)
{
    (void)self;
    return 0xC8;
}
