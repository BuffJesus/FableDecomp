#include "rebuild_abi.h"

// CHandleBasedMemoryPool::GetPointerFromHandle @ 0x00a026b0
// __fastcall: indexes the pointer table at this+0x438 by the handle, returns the
// slot pointer offset by +0x20 (past the block header). One stack arg, ret 4.
struct FablePool_00a026b0
{
    void* GetPointerFromHandle(int handle);
};

void* FablePool_00a026b0::GetPointerFromHandle(int handle)
{
    char** table = *(char***)((char*)this + 0x438);
    return table[handle] + 0x20;
}