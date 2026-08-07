#include "rebuild_abi.h"
#include <cstdio>
struct CWorld { char pad[0xa4]; char m_paused; void SetAsPaused(char v); };
void CWorld::SetAsPaused(char v){ if(v!=m_paused) m_paused=v; }
int main(){ CWorld o; o.m_paused=0; o.SetAsPaused(1);
 if(o.m_paused!=1){std::printf("B1\n");return 1;}
 std::printf("SP_49d8f0_OK\n"); return 0; }