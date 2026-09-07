#include "engine/CWorldMap.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorldMap_Methods : CWorldMap {
    void Impl(long a);
    void ForceUnload(long a);
};
void CWorldMap_Methods::ForceUnload(long a){ this->Impl(a); }