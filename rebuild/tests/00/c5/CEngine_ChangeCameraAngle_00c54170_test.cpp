#include "rebuild_abi.h"
#include <cstdio>
struct Sub584 { int hit; };
static Sub584 *g_last=0;
extern "C" void FABLE_FASTCALL FableSub_c54170(Sub584 *s){ g_last=s; s->hit=1; }
struct CEngine2 { char pad[4]; char m_flag; char pad2[0x584-5]; char sub[4]; void ChangeCameraAngle(); };
void CEngine2::ChangeCameraAngle(){ FableSub_c54170((Sub584*)&sub[0]); m_flag=0; }
int main(){ CEngine2 o; o.m_flag=1; o.ChangeCameraAngle();
 if(o.m_flag!=0||g_last!=(Sub584*)&o.sub[0]){std::printf("B1\n");return 1;}
 std::printf("CA_c54170_OK\n"); return 0; }