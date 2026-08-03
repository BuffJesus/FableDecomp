#pragma optimize("s",on)
#include "rebuild_abi.h"

// GFSerialiseVectorBinaryOut<float>  @ 0x004720f1
// this in ecx (ebp), vector range ptr in edx (edi).
//   count = (end - begin) >> 2   (4-byte element stride)
//   thisptr->WriteCount(count)             -> push esi; call
//   for i in [0,count): push [first+i*4]; call  (element pushed BY VALUE as a raw dword)
// The element sink takes the 4-byte value by value (plain integer push),
// mirroring the landed GFSerialiseVectorBinaryOut<CAtmosBankEntry> Serialise(int) idiom.

struct FloatVecRange {
    int* first;   // +0x00  begin
    int* last;    // +0x04  end
};

struct GFSerialiser {
    void WriteCount(int count);
    void WriteElem(int elem);
};

void __fastcall GFSerialiseVectorBinaryOut_float(GFSerialiser* thisptr, void* /*edx*/ vec)
{
    FloatVecRange* r = (FloatVecRange*)vec;
    int count = (int)(((char*)r->last - (char*)r->first) >> 2);
    thisptr->WriteCount(count);
    for (int i = 0; i < count; ++i) {
        thisptr->WriteElem(*(int*)((char*)r->first + i * 4));
    }
}