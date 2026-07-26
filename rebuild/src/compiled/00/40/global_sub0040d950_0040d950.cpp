#include "rebuild_abi.h"

// 0x0040d950 : mov eax, ecx ; ret
// __fastcall identity accessor: returns its first (ecx) argument unchanged.
// Models a this-returning member (e.g. `return this;`) authored as a free
// __fastcall taking the object pointer in ecx.
extern "C" void* __fastcall sub_0040d950(void* thisptr)
{
    return thisptr;
}