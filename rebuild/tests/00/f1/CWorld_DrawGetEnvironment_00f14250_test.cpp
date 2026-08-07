#include "rebuild_abi.h"
#include <cstdio>
struct CWorld { char pad[0x44]; void *m_env; void *DrawGetEnvironment(); };
void *CWorld::DrawGetEnvironment(){ return m_env; }
int main(){ CWorld o; int x; o.m_env=&x;
 if(o.DrawGetEnvironment()!=&x){std::printf("B1\n");return 1;}
 std::printf("DE_f14250_OK\n"); return 0; }