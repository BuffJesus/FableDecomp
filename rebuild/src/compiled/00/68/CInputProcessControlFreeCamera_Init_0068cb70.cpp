#include "engine/CInputProcessControlFreeCamera.h"  // retyped onto the PDB layout; byte parity re-verified
static char g_f=0;
struct CInputProcessControlFreeCamera_Methods : CInputProcessControlFreeCamera {
    void Init();
};
void CInputProcessControlFreeCamera_Methods::Init(){ if(!g_f) g_f=1; }