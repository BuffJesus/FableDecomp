#include "rebuild_abi.h"
#include <cstdio>
struct CEngine { char pad18[0x18]; char m_b18; char pad19[0xb]; char m_b24; bool IsPreloaded(); };
bool CEngine::IsPreloaded(){ if(m_b18||m_b24) return false; return true; }
int main(){ CEngine o; o.m_b18=0; o.m_b24=0; if(!o.IsPreloaded()){std::printf("B1\n");return 1;}
 o.m_b18=1; if(o.IsPreloaded()){std::printf("B2\n");return 1;}
 o.m_b18=0; o.m_b24=1; if(o.IsPreloaded()){std::printf("B3\n");return 1;}
 std::printf("PL_b23a70_OK\n"); return 0; }