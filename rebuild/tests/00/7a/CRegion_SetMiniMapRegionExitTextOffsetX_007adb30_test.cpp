#include "rebuild_abi.h"
#include <cstdio>
struct CRegion { void *m_p; };
extern "C" void *FABLE_STDCALL FableHelp_7adb30(void *v){ return (void*)((char*)v+1); }
extern "C" CRegion *FABLE_STDCALL FableSet_7adb30(CRegion *self, void *v){ self->m_p=FableHelp_7adb30(v); return self; }
int main(){ CRegion o; int x;
 if(FableSet_7adb30(&o,&x)!=&o||o.m_p!=(void*)((char*)&x+1)){std::printf("B1\n");return 1;}
 std::printf("MM_7adb30_OK\n"); return 0; }