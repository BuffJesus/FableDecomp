#include "rebuild_abi.h"
#include <cstdio>
struct State { char pad[8]; int m_prio; char pad2[0x10-0xc]; char m_flag; };
struct CAIBrain { char pad[0x38]; State *m_state; int GetPrio(); };
int CAIBrain::GetPrio(){ State *s=m_state; return (s && !s->m_flag) ? s->m_prio : 0; }
int main(){ State st; st.m_prio=42; st.m_flag=0; CAIBrain b; b.m_state=&st;
 if(b.GetPrio()!=42){std::printf("B1\n");return 1;}
 st.m_flag=1; if(b.GetPrio()!=0){std::printf("B2\n");return 1;}
 b.m_state=0; if(b.GetPrio()!=0){std::printf("B3\n");return 1;}
 std::printf("GP_88b760_OK\n"); return 0; }