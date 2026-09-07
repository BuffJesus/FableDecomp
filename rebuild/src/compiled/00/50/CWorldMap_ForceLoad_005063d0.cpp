#include "engine/CWorldMap.h"  // retyped onto the PDB layout; byte parity re-verified
extern void __stdcall fl_helper(long x, int z);
struct CWorldMap_Methods : CWorldMap {
    void ForceLoad(long x);
};
void CWorldMap_Methods::ForceLoad(long x){ fl_helper(x, 0); }