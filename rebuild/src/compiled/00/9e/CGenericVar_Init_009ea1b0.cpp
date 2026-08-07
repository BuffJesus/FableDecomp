#include "rebuild_abi.h"

// CGenericVar::Init @ 0x009ea1b0
// __fastcall: copies the value at *src into this+0xc, tags variant type
// (this+4) with 1. One stack arg, ret 4.
struct FableGenericVar_009ea1b0
{
    void Init(const void* src);
};

void FableGenericVar_009ea1b0::Init(const void* src)
{
        *(unsigned int*)((char*)this + 0xc) = *(const unsigned int*)src;
    *(unsigned int*)((char*)this + 4) = 1;
}