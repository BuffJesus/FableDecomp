#pragma optimize("s",on)
#include "rebuild_abi.h"

// GFSerialiseVectorBinaryOut<C2DVector>  @ 0x00475dc7
// Element stride is 8 bytes (C2DVector = two floats): retail computes the count
// with `sub esi,[edi]; sar esi,3` and indexes each element via
// `lea eax,[eax+ebx*8]`. `this` is stashed in ebp; the count and each element
// pointer are pushed on the stack (both callees are __fastcall sinks that take
// a single stack arg, `this` in ecx).

struct C2DVector {
    float x;  // +0x0
    float y;  // +0x4
};

struct VecRange {
    C2DVector* first;  // +0x00
    C2DVector* last;   // +0x04
};

struct GFSerialiser {
    void WriteCount(int count);       // 0x51e0e9  stack arg, this in ecx
    void WriteElem(C2DVector* elem);  // 0x51e1c9  stack arg, this in ecx
};

void __fastcall GFSerialiseVectorBinaryOut_C2DVector(GFSerialiser* thisptr, void* /*edx*/ vec)
{
    VecRange* r = (VecRange*)vec;
    int count = (int)(r->last - r->first);
    thisptr->WriteCount(count);
    for (int i = 0; i < count; ++i) {
        thisptr->WriteElem(r->first + i);
    }
}