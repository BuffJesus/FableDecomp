#include <stdio.h>
struct CPackedUIntArray { unsigned long* PackedInts; unsigned long Size;
 unsigned char Bits; unsigned char alignment[3]; unsigned long Bias;
 unsigned long Sizeof() const; };
unsigned long CPackedUIntArray::Sizeof() const {
 unsigned long packedBitCount=Size*Bits;
 return sizeof(CPackedUIntArray)+((packedBitCount+31)>>5)*sizeof(unsigned long);
}
int main() { CPackedUIntArray values;
 values.Size=0; values.Bits=7; if(values.Sizeof()!=16) return 1;
 values.Size=10; values.Bits=7; if(values.Sizeof()!=28) return 2;
 values.Size=32; values.Bits=1; if(values.Sizeof()!=20) return 3;
 printf("PACKED_UINT_ARRAY_SIZE PASS\n"); return 0; }