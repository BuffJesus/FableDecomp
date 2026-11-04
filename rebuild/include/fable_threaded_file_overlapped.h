#pragma once
#include "rebuild_abi.h"
#include <stddef.h>

// Original x86 Win32 ABI; Ego_r PDB: 20 bytes, hEvent at +16.
// The threaded manager stores its CFileObject pointer in hEvent for ReadFileEx.
struct _OVERLAPPED
{
    unsigned long Internal, InternalHigh;
    union
    {
        struct { unsigned long Offset, OffsetHigh; };
        void* Pointer;
    };
    void* hEvent;
};
FABLE_STATIC_ASSERT(sizeof(_OVERLAPPED) == 20);
FABLE_STATIC_ASSERT(offsetof(_OVERLAPPED, hEvent) == 16);
