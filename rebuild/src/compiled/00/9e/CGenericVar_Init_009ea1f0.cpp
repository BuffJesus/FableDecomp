#include "rebuild_abi.h"

// CGenericVar::Init @ 0x009ea1f0
// __fastcall: copies the value at *src into this+0x14, tags variant type
// (this+4) with 4. One stack arg, ret 4.
struct FableGenericVar_009ea1f0
{
    void Init(const void* src);
};

void FableGenericVar_009ea1f0::Init(const void* src)
{
        *(unsigned char*)((char*)this + 0x14) = *(const unsigned char*)src;
    *(unsigned int*)((char*)this + 4) = 4;
}