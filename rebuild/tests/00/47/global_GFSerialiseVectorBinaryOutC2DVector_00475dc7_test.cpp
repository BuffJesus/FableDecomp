#include <cstdio>
#include "rebuild_abi.h"

/* Retail advances the range in 8-byte strides (ebx*8, span>>3), so the
   effective element unit is 8 bytes (two floats); model the test container
   to match. */
struct C2DVector { float x; float y; };

struct VecRange {
    C2DVector* first;
    C2DVector* last;
};

struct GFSerialiser {
    int seenCount;
    int elemSum;
    void WriteCount(int count);
    void WriteElem(C2DVector* elem);
};

void GFSerialiser::WriteCount(int count) { seenCount = count; }
void GFSerialiser::WriteElem(C2DVector* elem) { elemSum += (int)elem->x + (int)elem->y; }

void __fastcall GFSerialiseVectorBinaryOut_C2DVector(GFSerialiser* thisptr, void* vec);

int main()
{
    C2DVector data[3];
    data[0].x = 10; data[0].y = 1;
    data[1].x = 20; data[1].y = 2;
    data[2].x = 30; data[2].y = 3;

    VecRange r;
    r.first = &data[0];
    r.last  = &data[3];

    GFSerialiser s;
    s.seenCount = -1;
    s.elemSum = 0;

    GFSerialiseVectorBinaryOut_C2DVector(&s, &r);

    if (s.seenCount == 3 && s.elemSum == 66)
        printf("GFSERIALISE_C2DVECTOR_475DC7_PASS\n");
    else
        printf("FAIL count=%d sum=%d\n", s.seenCount, s.elemSum);
    return 0;
}