#pragma optimize("s",on)
#include "rebuild_abi.h"

// GFSerialiseVectorBinaryOut<CAtmosBankEntry>  @ 0x0046c2d2
// Serialiser 'this' has two __fastcall sinks (args pushed on stack, this in ecx):
//   0x527bde : Serialise(int)    -- push [ebx]; call
//   0x527c0e : Serialise(float)  -- fild [ebx+4]; fstp [esp]; call

struct CAtmosBankEntry {
    int a;   // +0x0
    int b;   // +0x4  (serialised as float via fild)
};

struct GFVector {
    CAtmosBankEntry* begin; // +0x0
    CAtmosBankEntry* end;   // +0x4
};

struct Serialiser {
    void Serialise(int v);    // 0x527bde  (__fastcall, stack arg)
    void Serialise(float v);  // 0x527c0e  (__fastcall, stack arg)
};

void __fastcall GFSerialiseVectorBinaryOut_CAtmosBankEntry(Serialiser* thisptr, GFVector* vec)
{
    int count = (int)(vec->end - vec->begin);
    thisptr->Serialise(count);
    for (int i = 0; i < count; ++i) {
        CAtmosBankEntry* e = vec->begin + i;
        thisptr->Serialise(e->a);
        thisptr->Serialise((float)e->b);
    }
}