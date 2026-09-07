#include "engine/CHandleBasedMemoryPool.h"  // retyped onto the PDB layout; byte parity re-verified
struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    bool M(int x0, int x1);
};
bool CHandleBasedMemoryPool_Methods::M(int x0, int x1) { return true; }