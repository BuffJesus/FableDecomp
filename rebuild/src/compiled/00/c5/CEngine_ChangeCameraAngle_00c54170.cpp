#include "rebuild_abi.h"
// CEngine::ChangeCameraAngle @ 0x00c54170
struct Sub584;
extern "C" void FABLE_FASTCALL FableSub_c54170(Sub584 *s);
struct CEngine2 { char pad[4]; char m_flag; char pad2[0x584-5]; char sub[4]; void ChangeCameraAngle(); };
void CEngine2::ChangeCameraAngle(){ FableSub_c54170((Sub584*)&sub[0]); m_flag=0; }