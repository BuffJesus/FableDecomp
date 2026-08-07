#include "rebuild_abi.h"
#include <cstdio>
#include <cstring>
struct FableGenericVar_009ea190 { void Init(const void* src); };
void FableGenericVar_009ea190::Init(const void* src)
{
        *(unsigned int*)((char*)this + 0x18) = *(const unsigned int*)src;
    *(unsigned int*)((char*)this + 4) = 3;
}
int main() {
    unsigned char buf[0x40];
    std::memset(buf, 0, sizeof(buf));
    unsigned int val = 0xAABBCCDD;
    ((FableGenericVar_009ea190*)buf)->Init(&val);
    if (*(unsigned int*)(buf + 4) != 3) { std::printf("BAD_TAG\n"); return 1; }
    if (*(unsigned int*)(buf + 0x18) != 0xAABBCCDD) { std::printf("BAD_VAL\n"); return 1; }
    std::printf("GVINIT_009ea190_OK\n"); return 0;
}