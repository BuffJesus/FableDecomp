#include "rebuild_abi.h"
#include <cstdio>
struct CCP { void *m_a; int *m_ref; };
extern "C" void FABLE_FASTCALL FableCons_62ee60(CCP *dst, const CCP *src){
    if(!dst) return; dst->m_a=src->m_a; int *r=src->m_ref; dst->m_ref=r; if(r) ++*r; }
int main(){ int refc=1; CCP s; s.m_a=(void*)7; s.m_ref=&refc; CCP d={0,0};
 FableCons_62ee60(&d,&s);
 if(d.m_a!=(void*)7||d.m_ref!=&refc||refc!=2){std::printf("B1\n");return 1;}
 std::printf("CP_62ee60_OK\n"); return 0; }