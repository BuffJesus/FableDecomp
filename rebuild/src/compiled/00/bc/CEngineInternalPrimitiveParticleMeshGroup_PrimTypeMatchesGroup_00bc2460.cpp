#include "engine/CEngineInternalPrimitiveParticleMeshGroup.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineInternalPrimitiveParticleMeshGroup_Methods : CEngineInternalPrimitiveParticleMeshGroup {
    bool PrimTypeMatchesGroup(int t);
};
bool CEngineInternalPrimitiveParticleMeshGroup_Methods::PrimTypeMatchesGroup(int t){ return t == 23; }