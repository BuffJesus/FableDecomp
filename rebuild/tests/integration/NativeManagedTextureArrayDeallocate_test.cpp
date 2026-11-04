#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "managed_array_deallocate_retail.h"
#include "managed_array_deallocate_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall ArrayDeallocate(void*,void*);
static unsigned arena[4096],initial[4096],expected[8][4096],captures[8][4096],captured,mutation,base,vector,table[2];static bool active;
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void capture(){require(captured<8);memcpy(captures[captured++],arena,sizeof(arena));}
static void event(unsigned id,unsigned p){unsigned log=base+0x1800;unsigned n=word(log,0)++;require(n<6);word(log,4+n*8)=id;word(log,8+n*8)=p;capture();}
extern "C" void __fastcall DeleteOwner(void* self,void*){event(1,(unsigned)self);if(mutation==1)word(vector,4)=base+0x1000;if(mutation==2)word(vector,8)=0;if(mutation==3)word(base+0x400,4)=base+0xa00;if(mutation==4){word(base+0xa00,4)=0xffffffff;word(base+0x400,0)=base+0xb00;}}
extern "C" void __cdecl free(void* p){if(!active)return;event(2,(unsigned)p);if(mutation==5)word(vector,0)=base+0x1100;if(mutation==6)word(vector,4)=0xffffffff;if(mutation==7)word(vector,8)=0x80000000;}
static unsigned pattern(unsigned n){const unsigned values[]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
static void destroy(unsigned first,unsigned last){while(first!=last){unsigned p=word(first,0);if(p){--word(p,4);if(!word(p,4))DeleteOwner((void*)p,0);word(first,0)=0;}first+=4;}}
static void model(){unsigned start=word(vector,0),capacity=(word(vector,8)-start)>>2;if(capacity){unsigned finish=word(vector,4);unsigned result=start;destroy(result,finish);word(vector,4)=result;start=word(vector,0);word(vector,0)=0;finish=word(vector,4);word(vector,4)=0;word(vector,8)=0;destroy(start,finish);if(start)free((void*)start);}}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));for(unsigned f=0;f<4;++f){unsigned target=fixups[f].target==0xffffffff?(unsigned)&free:(unsigned)(code+fixups[f].target);*(unsigned*)(code+fixups[f].site)=target-(unsigned)(code+fixups[f].site+4);}table[0]=0;table[1]=(unsigned)DeleteOwner;unsigned cases=0,snapshots=0;const unsigned caps[]={0,1,4},sizes[]={0,1,3};
 for(unsigned ci=0;ci<3;++ci)for(unsigned si=0;si<3;++si)for(unsigned shape=0;shape<4;++shape)for(unsigned ref=0;ref<8;++ref)for(mutation=0;mutation<8;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){memset(arena,fill?0xa9:0,sizeof(arena));base=(unsigned)arena+(place?0x1800:0);vector=base+0x100;unsigned start=shape==0?0:base+0x400;word(vector,0)=start;word(vector,4)=shape==0?0:start+sizes[si]*4;word(vector,8)=shape==0?0:start+caps[ci]*4;for(unsigned i=0;i<3;++i){word(base+0x400,i*4)=shape==1?0:base+0x900+(shape==2?0:i)*0x100;unsigned p=base+0x900+i*0x100;word(p,0)=(unsigned)table;word(p,4)=pattern(ref);}memset((void*)(base+0x1800),0,64);memcpy(initial,arena,sizeof(arena));unsigned expectedCount=0;
  for(unsigned mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));captured=0;active=true;if(!mode)model();else(mode==1?((void(__fastcall*)(void*,void*))code):ArrayDeallocate)((void*)vector,0);active=false;capture();if(!mode){expectedCount=captured;memcpy(expected,captures,sizeof(expected));}else{require(captured==expectedCount);for(unsigned i=0;i<captured;++i){require(!memcmp(expected[i],captures[i],sizeof(arena)));++snapshots;}}}++cases;}
 VirtualFree(code,0,0x8000);printf("NATIVE_MANAGED_TEXTURE_ARRAY_DEALLOCATE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
