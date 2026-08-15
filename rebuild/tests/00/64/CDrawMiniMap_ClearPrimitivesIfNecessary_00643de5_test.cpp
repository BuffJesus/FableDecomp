
#include <cstdio>
struct CDrawMiniMap;
struct VT { void (*slot0)(CDrawMiniMap*); void (*slot4)(CDrawMiniMap*); };
struct M { VT* vptr; char pad0[0x18-4]; unsigned char f_18; char pad1[0x28-0x19]; int subtag; char pad2[0xeb-0x2c]; unsigned char f_eb; };
static int g_sub=0, g_slot4=0;
static void slot4impl(CDrawMiniMap*){ g_slot4++; }
static void clr(M* m){ if(!m->f_18) g_sub++; if(!m->f_eb) m->vptr->slot4((CDrawMiniMap*)m); }
int main(){ VT vt; vt.slot0=0; vt.slot4=slot4impl; M m; for(int i=0;i<(int)sizeof(m);++i) ((char*)&m)[i]=0; m.vptr=&vt;
  m.f_18=0; m.f_eb=0; g_sub=g_slot4=0; clr(&m); if(g_sub!=1||g_slot4!=1){printf("F1\n");return 1;}
  m.f_18=1; m.f_eb=1; g_sub=g_slot4=0; clr(&m); if(g_sub!=0||g_slot4!=0){printf("F2\n");return 1;}
  printf("CLEARPRIM_OK\n"); return 0; }