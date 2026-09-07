#include "engine/CEngineInternalPrimitiveParticleMesh.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineLocalLightArray;
struct CEngineInternalPrimitiveParticleMesh_Methods : CEngineInternalPrimitiveParticleMesh {
    CEngineLocalLightArray* M();
};
CEngineLocalLightArray* CEngineInternalPrimitiveParticleMesh_Methods::M() { return (CEngineLocalLightArray*)&this->RenderInfo_Lights; }