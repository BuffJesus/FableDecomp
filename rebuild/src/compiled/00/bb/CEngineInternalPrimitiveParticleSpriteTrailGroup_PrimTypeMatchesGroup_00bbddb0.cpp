#include "engine/CEngineInternalPrimitiveParticleSpriteTrailGroup.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineInternalPrimitiveParticleSpriteTrailGroup_Methods : CEngineInternalPrimitiveParticleSpriteTrailGroup {
    bool PrimTypeMatchesGroup(int t);
};
bool CEngineInternalPrimitiveParticleSpriteTrailGroup_Methods::PrimTypeMatchesGroup(int t){ return t == 20; }