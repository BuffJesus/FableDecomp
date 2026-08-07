#include "rebuild_abi.h"
#include <cstdio>
struct GTab { char pad[4]; char *m_data; };
static char g_names[256];
static GTab g_tab={ {0}, g_names };
GTab *g_typeTable_4c7e30=&g_tab;
struct CThing { char pad[0x10]; unsigned char m_type; char GetTypeName(); };
char CThing::GetTypeName(){ unsigned idx=m_type; char *t=g_typeTable_4c7e30->m_data; return t[idx*20+9]; }
int main(){ CThing o; o.m_type=2; g_names[2*20+9]='Z';
 if(o.GetTypeName()!='Z'){std::printf("B1\n");return 1;}
 std::printf("TN_4c7e30_OK\n"); return 0; }