#include "rebuild_abi.h"
#include <cstdio>
struct Entry { char pad[4]; int m_data; };
struct CGuiControlListBox { char pad[0x188]; Entry *m_sel; int GetData(); };
int CGuiControlListBox::GetData(){ Entry *e=m_sel; if(!e) return (int)0x80000000; return e->m_data; }
int main(){ CGuiControlListBox o; Entry e; e.m_data=77; o.m_sel=&e;
 if(o.GetData()!=77){std::printf("B1\n");return 1;}
 o.m_sel=0; if(o.GetData()!=(int)0x80000000){std::printf("B2\n");return 1;}
 std::printf("LB_c3ea60_OK\n"); return 0; }