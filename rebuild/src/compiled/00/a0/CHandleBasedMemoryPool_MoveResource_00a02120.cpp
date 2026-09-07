// CHandleBasedMemoryPool_Methods::MoveResource forwarder
// Retail forwards to a cdecl helper, ignoring this/p0/p1.

#include "engine/CHandleBasedMemoryPool.h"  // retyped onto the PDB layout; byte parity re-verified
extern "C" void __cdecl MoveResourceHelper(void *dst, void *mid, unsigned long n);

struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    void MoveResource(void *p0, long p1, void *p2, void *p3, unsigned long p4);
};

void CHandleBasedMemoryPool_Methods::MoveResource(void *p0, long p1, void *p2, void *p3, unsigned long p4)
{
    MoveResourceHelper(p3, p2, p4);
}