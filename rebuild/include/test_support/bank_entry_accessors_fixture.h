#include "fable_game_text_bank.h"
#include <stdio.h>
#include <string.h>

static unsigned long Decode(const unsigned long* data,unsigned long index,unsigned int bits,unsigned long bias) {
    unsigned long value=0;
    for (unsigned int i=0;i<bits;++i) {
        unsigned long offset=index*bits+i;
        value|=((data[offset/32]>>(offset%32))&1ul)<<i;
    }
    return value+bias;
}
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d\n",__LINE__); return 1; } } while(0)
int main() {
    unsigned long words[130];
    unsigned long seed=0x31579bdf;
    for (unsigned int i=0;i<130;++i) { seed=seed*1664525ul+1013904223ul; words[i]=seed; }
    CHECK(g_PackedUIntMasks_013979E8[0]==0);
    for (unsigned int width=1;width<=32;++width) CHECK(g_PackedUIntMasks_013979E8[width]==(~0ul>>(32-width)));
    unsigned long packedBytes[4]={0,0,0,0};
    CPackedUIntArray& packed=*reinterpret_cast<CPackedUIntArray*>(packedBytes);
    packed.PackedInts=words; packed.Size=128;
    unsigned int cases=0;
    for (unsigned int width=0;width<=32;++width) {
        packed.Bits=static_cast<unsigned char>(width);
        for (unsigned int b=0;b<3;++b) {
            packed.Bias=b==0 ? 0 : (b==1 ? 17 : 0xfffffff0ul);
            for (unsigned int index=0;index<128;++index) {
                CHECK(packed[index]==Decode(words,index,width,packed.Bias)); ++cases;
            }
        }
    }
    packed.PackedInts=0; packed.Bits=0; packed.Bias=999;
    CHECK(packed[0xfffffffful]==999);
    unsigned long bankBytes[sizeof(CBankFile)/4]; memset(bankBytes,0,sizeof(bankBytes));
    CBankFile& bank=*reinterpret_cast<CBankFile*>(bankBytes);
    CBankFile::CRuntimeData entries[8]; memset(entries,0,sizeof(entries));
    bank.RuntimeData._M_start=entries;
    entries[3].DataType=255; entries[3].DataSize=0x98765432;
    CHECK(bank.GetEntryDataType(3)==255 && bank.GetEntryDataSize(3)==0x98765432);
    bank.PackedDataType.PackedInts=words; bank.PackedDataType.Size=8; bank.PackedDataType.Bits=3; bank.PackedDataType.Bias=4;
    bank.PackedDataSize.PackedInts=words; bank.PackedDataSize.Size=8; bank.PackedDataSize.Bits=23; bank.PackedDataSize.Bias=51;
    for (unsigned long index=1;index<=8;++index) {
        CHECK(bank.GetEntryDataType(index)==Decode(words,index-1,3,4));
        CHECK(bank.GetEntryDataSize(index)==Decode(words,index-1,23,51));
    }
    printf("BANK_ENTRY_ACCESSORS_PASS packedCases=%u zeroWidthNoRead=1 runtimeFields=2 oneBasedPackedIndices=16\n",cases);
    return 0;
}

#ifdef FABLE_TEST_PACKED_READER
#include "../../src/compiled/00/9c/CBankFile_EntryAccessors_009cc210.cpp"
#else
#include "../../src/compiled/00/a6/CPackedUIntArray_Get_00a629f0.cpp"
#endif
