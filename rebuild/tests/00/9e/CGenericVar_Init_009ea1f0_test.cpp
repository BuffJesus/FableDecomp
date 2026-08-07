#include "rebuild_abi.h"
#include <cstdio>
#include <cstring>
struct FableGenericVar_009ea1f0 { void Init(const void* src); };
void FableGenericVar_009ea1f0::Init(const void* src)
{
        *(unsigned char*)((char*)this + 0x14) = *(const unsigned char*)src;
    *(unsigned int*)((char*)this + 4) = 4;
}
int main() {
    unsigned char buf[0x40];
    std::memset(buf, 0, sizeof(buf));
    unsigned int val = 0xAABBCCDD;
    ((FableGenericVar_009ea1f0*)buf)->Init(&val);
    if (*(unsigned int*)(buf + 4) != 4) { std::printf("BAD_TAG\n"); return 1; }
    if (buf[0x14] != 0xDD) { std::printf("BAD_VAL\n"); return 1; }
    std::printf("GVINIT_009ea1f0_OK\n"); return 0;
}