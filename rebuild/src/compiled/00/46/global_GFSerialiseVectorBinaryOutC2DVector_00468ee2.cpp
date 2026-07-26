#pragma optimize("s",on)
#include "rebuild_abi.h"

struct C2DVector;

struct VecRange {
    C2DVector* first;   // +0x00
    C2DVector* last;    // +0x04
};

struct GFSerialiser {
    void WriteCount(int count);
    void WriteElem(C2DVector* elem);
};

void __fastcall GFSerialiseVectorBinaryOut_C2DVector(GFSerialiser* thisptr, void* /*edx*/ vec)
{
    VecRange* r = (VecRange*)vec;
    int count = (int)(((char*)r->last - (char*)r->first) >> 2);
    thisptr->WriteCount(count);
    for (int i = 0; i < count; ++i) {
        thisptr->WriteElem((C2DVector*)((char*)r->first + i * 4));
    }
}