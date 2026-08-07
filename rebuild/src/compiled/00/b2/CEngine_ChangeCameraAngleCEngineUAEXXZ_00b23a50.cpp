#include "rebuild_abi.h"
// CEngine::ChangeCameraAngle @ 0x00b23a50
// push esi; esi=ecx; ecx=ds:[g]; call helper(g); [esi+0x39]=1; ret
struct GObj;
extern GObj *g_camThing_b23a50;
extern "C" void FABLE_FASTCALL FableCamCall_b23a50(GObj *self);
struct CEngine { char pad[0x39]; char m_flag; void ChangeCameraAngle(); };
void CEngine::ChangeCameraAngle()
{
    FableCamCall_b23a50(g_camThing_b23a50);
    m_flag = 1;
}