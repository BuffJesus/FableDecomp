#include "engine/CEngine.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
// CEngine_Methods::ChangeCameraAngle @ 0x00b23a50
// push esi; esi=ecx; ecx=ds:[g]; call helper(g); [esi+0x39]=1; ret
struct GObj;
extern GObj *g_camThing_b23a50;
extern "C" void FABLE_FASTCALL FableCamCall_b23a50(GObj *self);
struct CEngine_Methods : CEngine {
    void ChangeCameraAngle();
};
void CEngine_Methods::ChangeCameraAngle()
{
    FableCamCall_b23a50(g_camThing_b23a50);
    CameraAngleChanged = 1;
}