#pragma once

// FableWin ?GFFPUClear / ?GFFPUSetDoublePrecision / ?GFFPUSetRoundingNearest@@YIXXZ.
//
// Lionhead's x87 control-word helpers. In FableWin (debug, 0x018A6380..0x018A63D8)
// they are out-of-line functions whose bodies are `fnstcw [cw]; cw = ...; fldcw [cw]`
// on a 4-byte local; in retail they are inlined into their callers
// (CMainGameComponent::Update 0x00418289, UpdateForcedTickSpeed 0x00417747):
//   fnstcw word [cw]; mov eax,[cw]; and eax,~0x100; or eax,0x200; mov [cw],eax; fldcw word [cw]
//   fnstcw word [cw]; and byte [cw+1],0xF3; fldcw word [cw]
// GFFPUClear is empty in both builds.
//
// PURITY NOTE: VC7.1 has no intrinsic for fnstcw/fldcw (_control87 is a CRT
// call), so the retail source necessarily used inline asm here; these two
// helpers are the irreducible hand-written part, exactly as Lionhead wrote
// them. Everything calling them stays compiler-emitted C++.

__forceinline void GFFPUClear()
{
}

__forceinline void GFFPUSetDoublePrecision()
{
    unsigned long controlWord;
    __asm fnstcw word ptr controlWord
    controlWord = (controlWord & ~0x300UL) | 0x200UL;
    __asm fldcw word ptr controlWord
}

__forceinline void GFFPUSetRoundingNearest()
{
    unsigned long controlWord;
    __asm fnstcw word ptr controlWord
    controlWord &= ~0xC00UL;
    __asm fldcw word ptr controlWord
}
