#include "rebuild_abi.h"

// C3DAnimFileXSequenceChunk::GetChunkDescription @ 0x00aa49d0
// Retail: __fastcall virtual member returning a CCharString BY VALUE.
// ecx = this (C3DAnimFileXSequenceChunk, unused); the hidden return-buffer
// pointer is the first stack arg. Modeled as a free __fastcall so ecx (this)
// and edx are consumed/ignored and the return buffer arrives as the first stack
// slot ([esp+4] -> after push esi -> [esp+8], loaded into esi). Body constructs
// the CCharString from a relocated string constant with length -1 and returns
// the buffer ptr in eax.
// Shape: push esi; mov esi,[esp+8]; push -1; push STR; mov ecx,esi;
//        call ctor(0x99ebf0); mov eax,esi; pop esi; ret 4.

// CCharString ctor at 0x99ebf0: __fastcall (this in ecx), two callee-cleaned
// stack args (const char* lit, int len). Model as a real member so a genuine
// thiscall call site is emitted (ecx=this, edx untouched).
struct CCharString {
    int dummy;
    CCharString* ctor_99ebf0(const char* lit, int len);
};

extern "C" CCharString* FABLE_FASTCALL
FableGetChunkDescription_00aa49d0(void* thisptr, void* edx, CCharString* result)
{
    result->ctor_99ebf0((const char*)0x129e1c0, -1);
    return result;
}