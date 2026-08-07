#include "rebuild_abi.h"
#include <cstdio>
struct Slot8 { void *a; void *b; };
struct CEngineLandscapeRenderer { char pad[0x5e4]; Slot8 *m_arr; void OnReg(unsigned idx, void *val); };
void CEngineLandscapeRenderer::OnReg(unsigned idx, void *val){ ((void**)&m_arr[idx])[0]=val; }
int main(){ CEngineLandscapeRenderer o; Slot8 arr[4]={}; o.m_arr=arr; int x;
 o.OnReg(2,&x); if(arr[2].a!=&x){std::printf("B1\n");return 1;}
 std::printf("ZS_b67780_OK\n"); return 0; }