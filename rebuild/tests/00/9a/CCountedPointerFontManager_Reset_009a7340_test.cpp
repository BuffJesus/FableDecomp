#include <stdio.h>
#include <stdlib.h>
#include "fable_system_manager.h"
static CCPPointerInfo blocks[3] = {CCPPointerInfo(0,0),CCPPointerInfo(0,0),CCPPointerInfo(0,0)};
static CCountedPointer<CFontManager>* owner;
static int failAlloc,mode,released,allocated,freed;
static void* deleted;
static char objects[128];
void* __cdecl operator new(size_t n){if(owner){if(n!=12)abort();++allocated;return failAlloc?0:&blocks[2];}return malloc(n);}
void __cdecl operator delete(void* p){if(owner){++freed;deleted=p;}else free(p);}
static void __fastcall release(void* p){if(p!=objects)abort();++released;if(mode==1)owner->Info=&blocks[1];if(mode==2)owner->Info=0;owner->Data=(CFontManager*)(objects+96);}
int main(){unsigned long counts[]={0,0,1,2,0xffffffff};int cases=0;
for(int c=0;c<5;++c)for(int incoming=0;incoming<3;++incoming)for(int fail=0;fail<2;++fail)for(int m=0;m<3;++m){
 CCountedPointer<CFontManager> p;owner=&p;mode=m;failAlloc=fail;released=allocated=freed=0;deleted=0;
 blocks[0].RefCount=counts[c];blocks[0].DeleteFunc=release;blocks[0].Data=objects;
 blocks[1].RefCount=17;blocks[2].RefCount=55;
 p.Data=(CFontManager*)objects;p.Info=c?&blocks[0]:0;
 CFontManager* next=incoming?(CFontManager*)(objects+(incoming==1?0:64)):0;
 p.Reset(next);int dying=c==2;
 if(p.Data!=next||p.Info!=(incoming&&!fail?&blocks[2]:0)||released!=dying||freed!=dying||allocated!=(incoming!=0))return 1;
 if(c&&blocks[0].RefCount!=counts[c]-1)return 2;
 if(dying&&deleted!=(m==1?&blocks[1]:m==2?0:&blocks[0]))return 3;
 if(incoming&&!fail&&(blocks[2].RefCount!=1||blocks[2].Data!=next||!blocks[2].DeleteFunc))return 4;
 p.Data=0;p.Info=0;owner=0;++cases;
}printf("FONT_MANAGER_RESET_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
