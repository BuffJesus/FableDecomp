#include <stdio.h>
struct CStreamingFontBankStateBlock { int reserved; int crcData; int Helper(void*, int); int CreateCRC(); };
struct CShaderBankStateBlock { int reserved; int crcData; int Helper(void*, int); int CreateCRC(); };
struct CFontBankStateBlock { int reserved; int crcData; int Helper(void*, int); int CreateCRC(); };
int CStreamingFontBankStateBlock::Helper(void* data, int size) { return *(int*)data + size; }
int CShaderBankStateBlock::Helper(void* data, int size) { return *(int*)data + size; }
int CFontBankStateBlock::Helper(void* data, int size) { return *(int*)data + size; }
int CStreamingFontBankStateBlock::CreateCRC() { return Helper(&crcData, 0x10); }
int CShaderBankStateBlock::CreateCRC() { return Helper(&crcData, 8); }
int CFontBankStateBlock::CreateCRC() { return Helper(&crcData, 0x0c); }
int main() {
    CStreamingFontBankStateBlock a; CShaderBankStateBlock b; CFontBankStateBlock c;
    a.crcData = b.crcData = c.crcData = 7;
    if (a.CreateCRC() != 23 || b.CreateCRC() != 15 || c.CreateCRC() != 19) return 1;
    printf("CRC_STATE_BLOCKS PASS\n");
    return 0;
}