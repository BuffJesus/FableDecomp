#include "rebuild_abi.h"
#include <cstdio>
static int g_one=0;
extern "C" void FABLE_STDCALL FableCam_788198(void *p, int one){ g_one=one; }
struct CEngine3 { char pad[0x60]; char m_flag; void ChangeCameraAngle(); };
void CEngine3::ChangeCameraAngle(){ FableCam_788198((void*)0x13bae00,1); m_flag=1; }
int main(){ CEngine3 o; o.m_flag=0; o.ChangeCameraAngle();
 if(o.m_flag!=1||g_one!=1){std::printf("B1\n");return 1;}
 std::printf("CA_788198_OK\n"); return 0; }