#include "rebuild_abi.h"
#include <cstdio>
struct CStateGroup {
    virtual int v0(){return 100;} virtual int v1(){return 101;} virtual int v2(){return 102;}
    virtual int v3(){return 103;} virtual int v4(){return 104;}
};
struct CAIBrain { void *pad0; CStateGroup *m_group; int GetCurrentStateGroupDebugText(); };
int CAIBrain::GetCurrentStateGroupDebugText(){ if(m_group==0) return -1; return m_group->v4(); }
int main(){
    CAIBrain b; b.m_group=0;
    if(b.GetCurrentStateGroupDebugText()!=-1){std::printf("BADNULL\n");return 1;}
    CStateGroup g; b.m_group=&g;
    b.GetCurrentStateGroupDebugText();
    std::printf("AIDBG28D0_OK\n"); return 0;
}