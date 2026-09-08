#include "engine/CHandleBasedMemoryPool.h"

// The PDB identifies Handles as CArray<CHandleInfo> at +0x438. Its first
// dword is the backing array pointer; each four-byte CHandleInfo is a union
// whose allocated state stores the resource pointer.
struct CHandleAllocationRetail {
    void* Resource;
};

struct CHandleInfoRetail {
    CHandleAllocationRetail* Allocation;
};

struct CHandleInfoArrayRetail {
    CHandleInfoRetail* Data;
    CHandleInfoRetail* End;
    CHandleInfoRetail* CapacityEnd;
    void* Allocator;
};

struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    void InternalSetResource(long handle, void* resource);
};

void CHandleBasedMemoryPool_Methods::InternalSetResource(
    long handle, void* resource) {
    CHandleInfoArrayRetail* handles =
        reinterpret_cast<CHandleInfoArrayRetail*>(Handles);
    handles->Data[handle].Allocation->Resource = resource;
}