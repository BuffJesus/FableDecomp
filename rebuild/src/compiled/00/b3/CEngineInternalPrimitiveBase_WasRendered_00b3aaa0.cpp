#include "engine/CEngineInternalPrimitiveBase.h"  // retyped onto the PDB layout; byte parity re-verified
extern unsigned short g_frame;
struct CEngineInternalPrimitiveBase_Methods : CEngineInternalPrimitiveBase {
    int M();
};
int CEngineInternalPrimitiveBase_Methods::M(){ unsigned short v=this->LastFrameRendered; return v == g_frame; }