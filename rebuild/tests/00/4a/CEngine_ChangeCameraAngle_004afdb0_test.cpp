#include "rebuild_abi.h"
#include <cstdio>
struct CCameraSub;
static void *g_sub_4afdb0=0;
extern "C" void FABLE_FASTCALL FableCameraSubTick_4afdb0(CCameraSub *sub){ g_sub_4afdb0=sub; }
struct CEngine { char pad00[0x58]; char m_cameraSub[0x8f-0x58]; unsigned char m_changed; void ChangeCameraAngle(); };
void CEngine::ChangeCameraAngle(){ FableCameraSubTick_4afdb0((CCameraSub*)&m_cameraSub[0]); m_changed=1; }
int main(){
    CEngine e; e.m_changed=0; e.ChangeCameraAngle();
    if(e.m_changed!=1||g_sub_4afdb0!=(void*)&e.m_cameraSub[0]){std::printf("BAD\n");return 1;}
    std::printf("CAMANGLE_OK\n"); return 0;
}