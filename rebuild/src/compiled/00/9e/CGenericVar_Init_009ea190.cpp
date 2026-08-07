#include "rebuild_abi.h"

// CGenericVar::Init @ 0x009ea190
// __fastcall: copies the value at *src into this+0x18, tags variant type
// (this+4) with 3. One stack arg, ret 4.
struct FableGenericVar_009ea190
{
    void Init(const void* src);
};

void FableGenericVar_009ea190::Init(const void* src)
{
        *(unsigned int*)((char*)this + 0x18) = *(const unsigned int*)src;
    *(unsigned int*)((char*)this + 4) = 3;
}