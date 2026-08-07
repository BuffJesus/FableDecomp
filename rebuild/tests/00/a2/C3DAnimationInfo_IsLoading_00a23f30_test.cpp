#include "rebuild_abi.h"
#include <cstdio>
struct CLoader { char pad00[0x30]; void *m_state; };
struct C3DAnimationInfo { char pad00[0xc]; CLoader *m_loader; bool IsLoading(); };
bool C3DAnimationInfo::IsLoading(){ CLoader *l=m_loader; if(l && l->m_state) return true; return false; }
int main(){
    C3DAnimationInfo a; a.m_loader=0;
    if(a.IsLoading()){std::printf("B1\n");return 1;}
    CLoader l; l.m_state=0; a.m_loader=&l;
    if(a.IsLoading()){std::printf("B2\n");return 1;}
    l.m_state=(void*)1;
    if(!a.IsLoading()){std::printf("B3\n");return 1;}
    std::printf("ISLOADING_OK\n"); return 0;
}