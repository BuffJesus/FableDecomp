#include "rebuild_abi.h"

// CGenericVar::Init @ 0x009ea1d0
// __fastcall: copies the value at *src into this+0x10, tags variant type
// (this+4) with 2. One stack arg, ret 4.
struct FableGenericVar_009ea1d0
{
    void Init(const void* src);
};

void FableGenericVar_009ea1d0::Init(const void* src)
{
        *(unsigned int*)((char*)this + 0x10) = *(const unsigned int*)src;
    *(unsigned int*)((char*)this + 4) = 2;
}