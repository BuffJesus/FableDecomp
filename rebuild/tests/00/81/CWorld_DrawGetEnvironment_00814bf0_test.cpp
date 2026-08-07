#include "rebuild_abi.h"
#include <cstdio>
struct CWorld { char pad[0x18]; void *m_env; void *DrawGetEnvironment(); };
void *CWorld::DrawGetEnvironment(){ return m_env; }
int main(){ CWorld o; int x; o.m_env=&x;
 if(o.DrawGetEnvironment()!=&x){std::printf("B1\n");return 1;}
 std::printf("DE_814bf0_OK\n"); return 0; }