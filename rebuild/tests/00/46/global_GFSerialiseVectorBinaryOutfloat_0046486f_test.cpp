#include "rebuild_abi.h"
#include <stdio.h>

/* Retail advances the range in 4-byte strides (ebx*4, span>>2) and pushes each
   element as a raw dword value; model the element sink as WriteElem(int). */
struct FloatVecRange {
    int* first;
    int* last;
};

struct GFSerialiser {
    int  seenCount;
    int  elemSum;
    int  elemCnt;
    void WriteCount(int count);
    void WriteElem(int elem);
};

void GFSerialiser::WriteCount(int count) { seenCount = count; }
void GFSerialiser::WriteElem(int elem)   { elemSum += elem; ++elemCnt; }

void __fastcall GFSerialiseVectorBinaryOut_float(GFSerialiser* thisptr, void* vec);

int main(void)
{
    int data[3];
    data[0] = 10;
    data[1] = 20;
    data[2] = 30;

    FloatVecRange r;
    r.first = &data[0];
    r.last  = &data[3];

    GFSerialiser s;
    s.seenCount = -1;
    s.elemSum = 0;
    s.elemCnt = 0;

    GFSerialiseVectorBinaryOut_float(&s, &r);

    if (s.seenCount == 3 && s.elemCnt == 3 && s.elemSum == 60)
        printf("GFSERIALISE_VECTOR_FLOAT_TEST PASS\n");
    else
        printf("FAIL count=%d cnt=%d sum=%d\n", s.seenCount, s.elemCnt, s.elemSum);
    return 0;
}