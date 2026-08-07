#include "rebuild_abi.h"
#include <cstdio>
#include <cstring>
struct FableGenericVar_009ea1b0 { void Init(const void* src); };
void FableGenericVar_009ea1b0::Init(const void* src)
{
        *(unsigned int*)((char*)this + 0xc) = *(const unsigned int*)src;
    *(unsigned int*)((char*)this + 4) = 1;
}
int main() {
    unsigned char buf[0x40];
    std::memset(buf, 0, sizeof(buf));
    unsigned int val = 0xAABBCCDD;
    ((FableGenericVar_009ea1b0*)buf)->Init(&val);
    if (*(unsigned int*)(buf + 4) != 1) { std::printf("BAD_TAG\n"); return 1; }
    if (*(unsigned int*)(buf + 0xc) != 0xAABBCCDD) { std::printf("BAD_VAL\n"); return 1; }
    std::printf("GVINIT_009ea1b0_OK\n"); return 0;
}