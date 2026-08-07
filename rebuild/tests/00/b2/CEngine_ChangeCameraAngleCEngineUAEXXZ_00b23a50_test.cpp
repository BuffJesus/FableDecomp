#include "rebuild_abi.h"
#include <cstdio>
struct GObj { int hit; };
static GObj g_obj={0};
GObj *g_camThing_b23a50=&g_obj;
extern "C" void FABLE_FASTCALL FableCamCall_b23a50(GObj *self){ self->hit=1; }
struct CEngine { char pad[0x39]; char m_flag; void ChangeCameraAngle(); };
void CEngine::ChangeCameraAngle(){ FableCamCall_b23a50(g_camThing_b23a50); m_flag=1; }
int main(){ CEngine o; o.m_flag=0; o.ChangeCameraAngle();
 if(o.m_flag!=1||g_obj.hit!=1){std::printf("B1\n");return 1;}
 std::printf("CA_b23a50_OK\n"); return 0; }