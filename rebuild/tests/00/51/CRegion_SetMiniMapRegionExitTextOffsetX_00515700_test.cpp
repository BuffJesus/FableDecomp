#include "rebuild_abi.h"
#include <cstdio>
struct CRegion { void *m_p; };
static void *g_in=0;
extern "C" void *FABLE_STDCALL FableHelp_515700(void *v){ g_in=v; return (void*)((char*)v+1); }
extern "C" CRegion *FABLE_STDCALL FableSet_515700(CRegion *self, void *v){ self->m_p=FableHelp_515700(v); return self; }
int main(){ CRegion o; int x;
 if(FableSet_515700(&o,&x)!=&o){std::printf("B1\n");return 1;}
 if(o.m_p!=(void*)((char*)&x+1)){std::printf("B2\n");return 1;}
 std::printf("MM_515700_OK\n"); return 0; }