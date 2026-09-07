#include "engine/CWorldMap.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorldMap_Methods : CWorldMap {
    long Impl() const;
    long DrawGetFrame() const;
};
long CWorldMap_Methods::DrawGetFrame() const { return this->Impl(); }